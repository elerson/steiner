#include "euclideansteiner.h"
#include "fstream"
#include "efst.h"
#include "bbobstacle.h"

EuclideanSteiner::EuclideanSteiner()
{
    m_enviroment = NULL;
    m_clientset = NULL;
    m_polygonset = NULL;
    m_mapenvironment = NULL;
    m_mapclients = NULL;
    m_euclideantreeinfo = NULL;
    m_choosen = NULL;
    m_epsilon = 0.000000001;
}

VisibilityGraphData *EuclideanSteiner::createSteinerGraph(Polygon_2 *enviroment, QVector<QPoint> *clientset, QVector<Polygon_2> *polygonset)
{
    this->generateVisibility(enviroment,clientset,polygonset);
    return this->createGraph(clientset);
}

void EuclideanSteiner::generateVisibility(Polygon_2 *enviroment, QVector<QPoint> *clientset, QVector<Polygon_2> *polygonset)
{

    //clean all previous variables
    if(m_mapenvironment)
        delete m_mapenvironment;
    if(m_mapclients)
        delete m_mapclients;
    m_polygons.clear();
    m_clients.clear();

    m_enviroment = enviroment;
    m_clientset = clientset;
    m_polygonset = polygonset;

    // create the environment
    // create the enviroment polgons

    VisiLibity::Polygon polygon;
    //first the boudaries
    for(int i = 0; i < m_enviroment->size(); i++)
        polygon.push_back(VisiLibity::Point((*m_enviroment)[i].x(),(*m_enviroment)[i].y()));
    m_polygons.push_back(polygon);

    //now the rest of the enviroment
    for(int i = 0; i <m_polygonset->size(); i++){
        polygon.clear();
        for(int j = 0; j <m_polygonset->at(i).size(); j++){
            polygon.push_back(VisiLibity::Point(m_polygonset->at(i)[j].x(),m_polygonset->at(i)[j].y()));
        }
        m_polygons.push_back(polygon);
    }
    m_mapenvironment = new VisiLibity::Environment(m_polygons);

    //fill the guards/clients
    for(int i = 0; i < m_clientset->size(); i++){
        m_clients.push_back(VisiLibity::Point(m_clientset->at(i).x(),m_clientset->at(i).y()));
    }
    m_mapclients = new  VisiLibity::Guards(m_clients);

    if(!isEnvironmentValid() || !areClientsValid()){
        delete m_mapenvironment;
        delete m_mapclients;

        m_enviroment = NULL;
        m_clientset = NULL;
        m_polygonset = NULL;
        m_mapenvironment = NULL;
        m_mapclients = NULL;

        return ;
    }

    //create a vector with all points
    std::vector<VisiLibity::Point> allpoints;
    std::vector<int> allpointsidentifier;
    int identifiers = -1;  // this is used to differ clients and # polygons
    //for each client
    for(int i = 0; i < m_clientset->size(); i++){
        allpoints.push_back(VisiLibity::Point(m_clientset->at(i).x(),m_clientset->at(i).y()));
        allpointsidentifier.push_back(identifiers);
    }
    //for each obstacle
    for(int i = 0; i < m_polygonset->size(); i++){
        identifiers++;
        for(int j = 0; j < m_polygonset->at(i).size(); j++){
            allpoints.push_back(VisiLibity::Point(m_polygonset->at(i)[j].x(),m_polygonset->at(i)[j].y()));
            allpointsidentifier.push_back(identifiers);
        }
    }

    //fill the guards/clients
    //calculate graph
    std::vector<VisiLibity::Polyline> visibilitylines;
    VisiLibityPointsGraph visibilitypointgraph(allpoints.size());
    for(int i = 0; i < allpoints.size(); i++){
        for(int j = 0; j < allpoints.size(); j++){
            if(i == j)
                continue;
            VisiLibity::Polyline shortestpath = m_mapenvironment->shortest_path(allpoints.at(i), allpoints.at(j), m_epsilon);
            visibilitylines.push_back(shortestpath);
            visibilitypointgraph.add(shortestpath,allpoints, ((i < m_clientset->size())&&(j < m_clientset->size())));
        }
    }

    visibilitypointgraph.printAll("outGerador",polygonset->size(),allpoints,allpointsidentifier);
    m_euclideantreeinfo = buildSteinerTrees("outGerador");
    m_choosen = buidBestTree_b(m_euclideantreeinfo,-100);


}

void EuclideanSteiner::draw(QPainter &painter, QPoint displacement, double aspectratio)
{
    if(!m_choosen) return;

    if(m_euclideantreeinfo){

        for(int i=0;i<m_euclideantreeinfo->num_edges;i++){

            if(m_choosen[i] == 1)
            {

                for(int j =0;j < m_euclideantreeinfo->full_trees[i]->nedges;j++){

                    int p1index = m_euclideantreeinfo->full_trees[i]->edges[j].p1;
                    int p2index = m_euclideantreeinfo->full_trees[i]->edges[j].p2;
                    int p1x,p1y;

                    if(p1index < m_euclideantreeinfo->full_trees[i]->terminals->n )                    {
                        p1x = m_euclideantreeinfo->full_trees[i]->terminals->a[p1index].x;
                        p1y = m_euclideantreeinfo->full_trees[i]->terminals->a[p1index].y;
                        std::cout  << "Terminal" << std::endl;
                    }else{
                        p1x = m_euclideantreeinfo->full_trees[i]->steiners->a[p1index - m_euclideantreeinfo->full_trees[i]->terminals->n].x;
                        p1y = m_euclideantreeinfo->full_trees[i]->steiners->a[p1index - m_euclideantreeinfo->full_trees[i]->terminals->n].y;
                        std::cout  << "Steiner" << std::endl;

                        // printf("e[%d %d %d %d]",x,y,xindice, info->full_trees[i]->terminals->n);
                    }
                    int p2x,p2y;
                    if(p2index < m_euclideantreeinfo->full_trees[i]->terminals->n )                    {
                        p2x = m_euclideantreeinfo->full_trees[i]->terminals->a[p2index].x;
                        p2y = m_euclideantreeinfo->full_trees[i]->terminals->a[p2index].y;
                        std::cout  << "Terminal" << std::endl;
                    }else{
                        p2x = m_euclideantreeinfo->full_trees[i]->steiners->a[p2index - m_euclideantreeinfo->full_trees[i]->terminals->n].x;
                        p2y = m_euclideantreeinfo->full_trees[i]->steiners->a[p2index - m_euclideantreeinfo->full_trees[i]->terminals->n].y;
                        std::cout  << "Steiner" << std::endl;
                    }
                    painter.drawLine(QPoint(p1x,p1y)*aspectratio+ displacement,QPoint(p2x,p2y)*aspectratio + displacement);
                }
            }
        }
    }
}

bool EuclideanSteiner::isEnvironmentValid()
{
    return m_mapenvironment->is_valid(m_epsilon);
}

bool EuclideanSteiner::areClientsValid()
{
    for(unsigned i=0; i<m_mapclients->N(); i++){
        if( !(*m_mapclients)[i].in(*m_mapenvironment, m_epsilon) ){
            return false;
        }
    }
    return true;
}

VisiLibityPointsGraph::VisiLibityPointsGraph(int size){
    m_size = size;
    m_visibilitygraph = new bool*[size];
    m_distancegraph = new int*[size];
    for(int i = 0; i < size; i++){
        m_visibilitygraph[i] = new bool[size];
        m_distancegraph[i] = new int[size];
    }

    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            m_visibilitygraph[i][j] = false;
            m_distancegraph[i][j] = 0;
        }
    }
}

void VisiLibityPointsGraph::add(VisiLibity::Polyline & line, std::vector<VisiLibity::Point> &allpoints,bool bothterminal){

    if(bothterminal){
        for(int i = 0; i < line.size()-1; i++){
            VisiLibity::Point linepoint1 = line[i];
            VisiLibity::Point linepoint2 = line[i+1];
            int p1mach = -1;
            for(int j = 0; j < allpoints.size(); j++){
                if(allpoints.at(j) == linepoint1){
                    p1mach = j;
                    break;
                }
            }

            int p2mach = -1;
            for(int j = 0; j < allpoints.size(); j++){
                if(allpoints.at(j) == linepoint2){
                    p2mach = j;
                    break;
                }
            }
            m_visibilitygraph[p1mach][p2mach] = true;
            m_visibilitygraph[p2mach][p1mach] = true;

        }
    }
    for(int i = 0; i < allpoints.size(); i++){
        for(int j = i+1; j < allpoints.size(); j++){
            VisiLibity::Point p = allpoints.at(i) - allpoints.at(j);
            double distance = sqrt((p.x()*p.x()) + (p.y()*p.y()) );
            m_distancegraph[i][j] = distance;
            m_distancegraph[j][i] = distance;
        }
    }



}

void VisiLibityPointsGraph::printAll(std::string filename,int numpolygons, std::vector<VisiLibity::Point>& points, std::vector<int>& index)
{
    std::ofstream outFile;
    outFile.open (filename.c_str());
    //print points
    outFile << points.size() << std::endl;
    outFile << numpolygons + 1 << std::endl;

    for(int i = 0; i < points.size(); i++){
        outFile << points.at(i).x() << " " <<points.at(i).y()<< " "<< index.at(i) << " 1" << std::endl;
    }
    //print matrix

    for(int i = 0; i < m_size; i++){
        for(int j = 0; j < m_size; j++){
            outFile << m_visibilitygraph[i][j] << " ";
        }
        outFile << std::endl;
    }

    for(int i = 0; i < m_size; i++){
        for(int j = 0; j < m_size; j++){
            outFile << m_distancegraph[i][j] << " ";
        }
        if(i < (m_size -1))
            outFile << std::endl;
    }
}

VisibilityGraphData *EuclideanSteiner::createGraph(QVector<QPoint> *clientset)
{

    if(!m_choosen) return NULL;
    if(!m_euclideantreeinfo) return NULL;



    PointsBag* pointbag = new PointsBag;

    if(m_euclideantreeinfo){

        for(int i=0;i<m_euclideantreeinfo->num_edges;i++){          
            if(m_choosen[i] == 1)
            {

                for(int j =0;j < m_euclideantreeinfo->full_trees[i]->nedges;j++){

                    int p1index = m_euclideantreeinfo->full_trees[i]->edges[j].p1;
                    int p2index = m_euclideantreeinfo->full_trees[i]->edges[j].p2;
                    int p1x,p1y;

                    if(p1index < m_euclideantreeinfo->full_trees[i]->terminals->n )                    {
                        p1x = m_euclideantreeinfo->full_trees[i]->terminals->a[p1index].x;
                        p1y = m_euclideantreeinfo->full_trees[i]->terminals->a[p1index].y;
                    }else{
                        p1x = m_euclideantreeinfo->full_trees[i]->steiners->a[p1index - m_euclideantreeinfo->full_trees[i]->terminals->n].x;
                        p1y = m_euclideantreeinfo->full_trees[i]->steiners->a[p1index - m_euclideantreeinfo->full_trees[i]->terminals->n].y;
                    }
                    int p2x,p2y;
                    if(p2index < m_euclideantreeinfo->full_trees[i]->terminals->n )                    {
                        p2x = m_euclideantreeinfo->full_trees[i]->terminals->a[p2index].x;
                        p2y = m_euclideantreeinfo->full_trees[i]->terminals->a[p2index].y;

                    }else{
                        p2x = m_euclideantreeinfo->full_trees[i]->steiners->a[p2index - m_euclideantreeinfo->full_trees[i]->terminals->n].x;
                        p2y = m_euclideantreeinfo->full_trees[i]->steiners->a[p2index - m_euclideantreeinfo->full_trees[i]->terminals->n].y;

                    }
                    VisiLibity::Point p1(p1x,p1y);
                    VisiLibity::Point p2(p2x,p2y);
                    pointbag->add(p1);
                    pointbag->add(p2);

                }
            }
        }

        AdjList* graph = new AdjList(pointbag->getSize(),false);

        for(int i=0;i<m_euclideantreeinfo->num_edges;i++){

            if(m_choosen[i] == 1){

                for(int j =0;j < m_euclideantreeinfo->full_trees[i]->nedges;j++){

                    int p1index = m_euclideantreeinfo->full_trees[i]->edges[j].p1;
                    int p2index = m_euclideantreeinfo->full_trees[i]->edges[j].p2;
                    int p1x,p1y;

                    if(p1index < m_euclideantreeinfo->full_trees[i]->terminals->n )                    {
                        p1x = m_euclideantreeinfo->full_trees[i]->terminals->a[p1index].x;
                        p1y = m_euclideantreeinfo->full_trees[i]->terminals->a[p1index].y;

                    }else{
                        p1x = m_euclideantreeinfo->full_trees[i]->steiners->a[p1index - m_euclideantreeinfo->full_trees[i]->terminals->n].x;
                        p1y = m_euclideantreeinfo->full_trees[i]->steiners->a[p1index - m_euclideantreeinfo->full_trees[i]->terminals->n].y;


                        // printf("e[%d %d %d %d]",x,y,xindice, info->full_trees[i]->terminals->n);
                    }
                    int p2x,p2y;
                    if(p2index < m_euclideantreeinfo->full_trees[i]->terminals->n )                    {
                        p2x = m_euclideantreeinfo->full_trees[i]->terminals->a[p2index].x;
                        p2y = m_euclideantreeinfo->full_trees[i]->terminals->a[p2index].y;

                    }else{
                        p2x = m_euclideantreeinfo->full_trees[i]->steiners->a[p2index - m_euclideantreeinfo->full_trees[i]->terminals->n].x;
                        p2y = m_euclideantreeinfo->full_trees[i]->steiners->a[p2index - m_euclideantreeinfo->full_trees[i]->terminals->n].y;

                    }
                    VisiLibity::Point p1(p1x,p1y);
                    VisiLibity::Point p2(p2x,p2y);
                    int p1n = pointbag->takeIndex(p1);
                    int p2n = pointbag->takeIndex(p2);
                    double distance = pointbag->getDistance(p1n,p2n);
                    graph->add(p1n,new GraphNode(p2n,distance));

                }
            }
        }

        VisibilityGraphData *graphdata= new VisibilityGraphData;

        graphdata->graph = graph;
        graphdata->pointBag = pointbag;
        graphdata->pointBag->generateTerminalPoints(clientset);
        return graphdata;
    }

    return NULL;



}
