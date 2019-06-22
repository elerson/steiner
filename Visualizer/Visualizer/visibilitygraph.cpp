#include "visibilitygraph.h"
#include <iostream>
#include <fstream>

VisibilityGraph::VisibilityGraph()
{
    m_enviroment = NULL;
    m_clientset = NULL;
    m_polygonset = NULL;
    m_mapenvironment = NULL;
    m_mapclients = NULL;

    m_epsilon = 0.000000001;
}

bool VisibilityGraph::isEnvironmentValid()
{
    return m_mapenvironment->is_valid(m_epsilon);
}

bool VisibilityGraph::areClientsValid()
{
    for(unsigned i=0; i<m_mapclients->N(); i++){
        if( !(*m_mapclients)[i].in(*m_mapenvironment, m_epsilon) ){
            return false;
        }
    }
    return true;
}

VisibilityGraphData* VisibilityGraph::createVisibilityGraph(Polygon_2* enviroment, QVector<QPoint>* clientset,  QVector< Polygon_2 >* polygonset)
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

        return NULL;
    }

    //fill the guards/clients
    //calculate graph
    std::vector<VisiLibity::Polyline> visibilitylines;
    PointsBag* pointbag = new PointsBag;
    for(int i = 0; i < m_clients.size(); i++){
        for(int j = 0; j < m_clients.size(); j++){
            if(i == j)
                continue;
            VisiLibity::Polyline shortestpath = m_mapenvironment->shortest_path(m_clients.at(i), m_clients.at(j), m_epsilon);
            pointbag->add(shortestpath);
            visibilitylines.push_back(shortestpath);

        }
    }
    AdjList* graph = new AdjList(pointbag->getSize(),true);
    for(int i = 0; i < visibilitylines.size(); i++)
        for(int k = 0; k < visibilitylines.at(i).size() -1; k++){
            int p1 = pointbag->takeIndex(visibilitylines.at(i)[k]);
            int p2 = pointbag->takeIndex(visibilitylines.at(i)[k+1]);
            double distance = pointbag->getDistance(p1,p2);
            graph->add(p1,new GraphNode(p2,distance));
            //std::cout << p1 << "->" << p2 << " "  << distance << std::endl;
        }

    VisibilityGraphData *graphdata= new VisibilityGraphData;

    graphdata->graph = graph;
    graphdata->pointBag = pointbag;
    graphdata->pointBag->generateTerminalPoints(clientset);
    return graphdata;
}

PointsBag::PointsBag()
{

}

bool ComparePoints(VisiLibity::Point& p1, VisiLibity::Point& p2){
    if(abs((p1.x() - p2.x())) < COMPMINDIST)
        if(abs((p1.y() - p2.y())) < COMPMINDIST)
            return true;
    return false;
}

void PointsBag::add(VisiLibity::Polyline &polyline)
{
    for(int i = 0; i < polyline.size(); i++){
        bool newpoint = true;
        for(int j = 0; j < m_bag.size(); j++){
            if(ComparePoints(m_bag.at(j),polyline[i])){
                newpoint = false;
                break;
            }
        }
        if(newpoint)
            m_bag.push_back(polyline[i]);
    }
}

void PointsBag::add(VisiLibity::Point &point)
{
    bool newpoint = true;
    for(int j = 0; j < m_bag.size(); j++){
        if(ComparePoints(m_bag.at(j),point)){
            newpoint = false;
            break;
        }
    }
    if(newpoint)
        m_bag.push_back(point);
}

int PointsBag::takeIndex(VisiLibity::Point &p)
{
    for(int j = 0; j < m_bag.size(); j++){
        if(ComparePoints(m_bag.at(j),p)){
            return j;
        }
    }
    return -1;
}

int PointsBag::getSize()
{
    return m_bag.size();
}

double PointsBag::getDistance(int p1, int p2)
{
    VisiLibity::Point distance = m_bag.at(p1) - m_bag.at(p2);
    return sqrt(distance.x()*distance.x() + distance.y()*distance.y());
}

QPoint PointsBag::getdata(int i)
{
    QPoint p(m_bag.at(i).x(),m_bag.at(i).y());
    return p;
}

void PointsBag::generateTerminalPoints(QVector<QPoint> *clientset)
{
    for(int i = 0; i < clientset->size(); i++){
        for(int j = 0; j < m_bag.size(); j++){
            VisiLibity::Point p2(clientset->at(i).x(),clientset->at(i).y());
            if(ComparePoints(m_bag.at(j),p2)){
                m_terminals.push_back(j);
            }
        }
    }
}

std::vector<int> &PointsBag::getTerminals()
{
    return m_terminals;
}

double VisibilityGraphData::getTreeSize()
{
    double size = 0;
    int seg = 0;
    for(int i = 0; i < graph->size; i++){
        for(int j = 0; j < graph->List[i]->size();j++){
            int p1 = i;
            int p2 = graph->List[i]->at(j)->j;
            QPoint d = pointBag->getdata(p1) - pointBag->getdata(p2);
            size += sqrt(d.x()*d.x() + d.y()*d.y());
            seg++;
        }
    }
    std::cout <<"SEGMENTOS "<< seg << std::endl;
    return size;
}

void VisibilityGraphData::saveFile1(std::string file)
{

    file.append("steinerData1.dat");
    std::ofstream file1(file.c_str());
    // create newgraph
    AdjList* newgraph = new AdjList(graph->size,true);
    for(int i = 0; i < graph->size; i++){

        for(int j = 0; j < graph->List[i]->size();j++){
            int p1 = i;
            int p2 = graph->List[i]->at(j)->j;
            double w = graph->List[i]->at(j)->w;
            newgraph->add(p1,new GraphNode(p2,w));
        }
    }


    //save graph
    for(int i = 0; i < newgraph->size; i++){
        file1 << i << std::endl;
        for(int j = 0; j < newgraph->List[i]->size();j++){
            int p2 = newgraph->List[i]->at(j)->j;
            file1 << p2 << " ";
        }
        file1 <<"-1"<<std::endl;
    }
    file1 <<"-2"<<std::endl;

    //save vertices
    for(int i = 0 ; i < pointBag->getSize(); i++){
        file1 <<  i << " " << pointBag->getdata(i).x() << " " << pointBag->getdata(i).y() << " ";
    }
    file1 <<"-2"<<std::endl;
    file1.close();
}

void VisibilityGraphData::saveFile2(std::string file)
{
    file.append("steinerData2.dat");
    std::ofstream file2(file.c_str());

    int* usedPoints;
    int* usedPointsmap;
    int* gateway;
    int numPoints;

    numPoints = pointBag->getSize();

    usedPoints = new int[numPoints];
    usedPointsmap = new int[numPoints];
    gateway = new int[numPoints];

    std::vector<int> terminals = pointBag->getTerminals();

    for(int i = 0 ; i < numPoints; i++){
        usedPoints[i] = 1;
        usedPointsmap[i] = i;
        int terminal = 0;
        for(int j = 0 ; j < terminals.size(); j++){
            if(i == terminals.at(j)){
               terminal = 1;
               break;
            }
        }
        gateway[i] = terminal;
    }

    file2.write((char*)&numPoints,sizeof(int));
    file2.write((char*)usedPoints,numPoints*sizeof(int));
    file2.write((char*)usedPointsmap,numPoints*sizeof(int));
    file2.write((char*)gateway,numPoints*sizeof(int));

    file2.close();

}

void VisibilityGraphData::saveFile3(std::string file)
{
    file.append("steinerData3.dat");
    std::ofstream file3(file.c_str());

    file3 << "Tree Size " <<this->getTreeSize() << std::endl;
    file3 << "Points " <<this->pointBag->getSize() << std::endl;
    file3 << "Terminals " <<this->pointBag->getTerminals().size() << std::endl;
}
