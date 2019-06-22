#include "image.h"
#include <iostream>
#include <QDebug>
#include <visibilitygraph.h>
#include <visilibity.hpp>
#include "SteinerApproximation.h"
#include "euclideansteiner.h"
#include <vector>
#include <QDir>


Image::Image(QWidget *parent) :
    QWidget(parent)
{}

void Image::initialize(std::string image_file){
    m_image.load(image_file.c_str());
    cv_img = cv::imread(image_file, 0);

    this->createObstacles();
    this->createEnvironment();
    m_ddpoint.setX(0);
    m_ddpoint.setY(0);
    m_zoom.setX(0);
    m_zoom.setY(0);
    m_graphdata = NULL;

    m_euclideansteiner = new EuclideanSteiner;

    m_drawclientsize.setX(CLIENTSIZE);
    m_drawclientsize.setY(CLIENTSIZE);

    m_drawpoligonsize.setX(POLYGONSIZE);
    m_drawpoligonsize.setY(POLYGONSIZE);

    //mouse drag

    m_mousedragfunctions.push_back(&Image::dragPicture);
    m_mousedragfunctions.push_back(&Image::createClient);
    m_mousedragfunctions.push_back(&Image::createPolygon);
    m_mousedragfunctions.push_back(&Image::deletePolygon);
    m_mousedragfunctions.push_back(&Image::deleteClient);


    //mouse move
    m_mousemovefunctions.push_back( &Image::movePicture);

    //mouse drop
    m_mousedropfunctions.push_back(&Image::dropPicture);

    //draw functions
    m_drawfunctions.push_back(&Image::drawImage);
    m_drawfunctions.push_back(&Image::drawClients);
    m_drawfunctions.push_back(&Image::drawPolygons);
    m_drawfunctions.push_back(&Image::drawGraph);
    m_drawfunctions.push_back(&Image::drawExternal);

    m_aspectratio = 1;
    m_selection = CREATECLIENT;
    m_tree = GRAPHSTEINER;

}

void Image::createObstacles(){

    // int currScale = _params->adaptiveThreshWinSizeMin + i*_params->adaptiveThreshWinSizeStep;
    cv::Mat thresh;
    cv::threshold(cv_img, thresh, 127, 255, cv::THRESH_BINARY);

    std::vector< std::vector< cv::Point > > contours_p;
    cv::findContours(thresh, contours_p, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

    


    /// Find the convex hull object for each contour
    std::vector<std::vector<cv::Point> > hull( contours_p.size() );


    for( int i = 0; i < contours_p.size(); i++ )
      {  cv::convexHull( cv::Mat(contours_p[i]), hull[i], true ); }


    std::vector<std::vector<cv::Point> > obstacles( contours_p.size() );
    for( int i = 0; i < hull.size()-1; i++ ){
        
        for(auto new_point: hull[i]){
            bool is_distant = true;
            for(auto point: obstacles[i]){
                double dist = sqrt((point.x-new_point.x)*(point.x-new_point.x) + (point.y-new_point.y)*(point.y-new_point.y));
                std::cout << new_point << "  " << point << " " << dist << std::endl;
                if(dist < 50){ //50 pixels
                    is_distant = false;
                    break;   
                }
            }
            if(is_distant){
                obstacles[i].push_back(new_point);
            }
        }

        Polygon_2 polygon;
        for(auto point : obstacles[i]){
            cv::circle(cv_img, point, 4, cv::Scalar(0,0,255));
            polygon.push_back(Point(point.x,point.y));
        }
        m_polygonset.push_back(polygon);
        std::cout << std::endl;
    }
    

    //cv::imwrite("thresh.png", cv_img);

    

}

void Image::paintEvent(QPaintEvent *event)
{
    void (Image::*funcPtr)( void *);
    for (int i = 0; i < m_drawfunctions.size(); ++i) {
        funcPtr = m_drawfunctions.at(i);
        (this->*funcPtr)((void*)event);
    }

}

void Image::dragEnterEvent(QDragEnterEvent *)
{



}

void Image::dragMoveEvent(QDragMoveEvent *)
{

}

void Image::mousePressEvent(QMouseEvent *event)
{
    void (Image::*funcPtr)( void *) = &Image::dragPicture;
    for (int i = 0; i < m_mousedragfunctions.size(); ++i) {
        funcPtr = m_mousedragfunctions.at(i);
        (this->*funcPtr)((void*)event);
    }
    this->setFocus();
}

void Image::mouseMoveEvent(QMouseEvent *event)
{
    void (Image::*funcPtr)( void *) = &Image::dragPicture;
    for (int i = 0; i < m_mousemovefunctions.size(); ++i) {
        funcPtr = m_mousemovefunctions.at(i);
        (this->*funcPtr)((void*)event);
    }

}

void Image::mouseReleaseEvent(QMouseEvent *event)
{
    void (Image::*funcPtr)( void *) = &Image::dragPicture;
    for (int i = 0; i < m_mousedropfunctions.size(); ++i) {
        funcPtr = m_mousedropfunctions.at(i);
        (this->*funcPtr)((void*)event);
    }
}

void Image::wheelEvent(QWheelEvent *event)
{
    if(event->delta() > 0)
        m_aspectratio += SENSIBILITY;
    else
        m_aspectratio -= SENSIBILITY;
    repaint();
}

void Image::keyPressEvent(QKeyEvent *event)
{

    if(event->key() == Qt::Key_Delete){
        m_polygoninconstruction.pop_back();
        repaint();
    }
    if(event->key() == Qt::Key_C){

    }
}

void Image::drawImage(void *)
{
    QPainter painter(this);
    painter.drawImage(m_ddpoint,m_image.scaled( m_image.width()*m_aspectratio, m_image.height()*m_aspectratio));
}

void Image::dragPicture(void *vevent)
{
    QMouseEvent *event = (QMouseEvent*) vevent;
    if (event->button() == Qt::LeftButton) {
        m_dragpos = event->pos();
        m_dragddpoint = m_ddpoint;
        QApplication::setOverrideCursor(QCursor(Qt::ClosedHandCursor));
    }
}

void Image::movePicture(void *vevent)
{
    QMouseEvent *event = (QMouseEvent*) vevent;
    if ((event->buttons() == Qt::LeftButton))
    {

        m_maxdiff.setX((this->height()) - m_image.height()*m_aspectratio);
        m_maxdiff.setY((this->width()) - m_image.width()*m_aspectratio);

        QPoint distance;
        distance = m_dragpos -  event->pos();
        QPoint ddresult = m_dragddpoint - distance;

        if((ddresult.x() > m_maxdiff.x())&&(ddresult.x() < 0))
            m_ddpoint.setX(ddresult.x());

        if((ddresult.y() > m_maxdiff.y())&&(ddresult.y() < 0))
            m_ddpoint.setY(ddresult.y());
    }

    repaint();
}

void Image::dropPicture(void *vevent)
{
    QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
}

void Image::createClient(void *vevent)
{
    QMouseEvent *event = (QMouseEvent*) vevent;
    if ((event->buttons() == Qt::RightButton) && (m_selection == CREATECLIENT))
    {
        QPoint pointposition  = event->pos()/m_aspectratio - m_ddpoint;
        m_clientset.push_back(pointposition);
        repaint();
    }
}

void Image::createEnvironment(){
    int x_min = 0;
    int y_min = m_image.height();
    int x_max = m_image.width();
    int y_max = 0;
    m_environment.push_back(Point(x_min, y_min));
    m_environment.push_back(Point(x_min, y_max));
    m_environment.push_back(Point(x_max, y_max));
    m_environment.push_back(Point(x_max, y_min));
}

void Image::createPolygon(void *vevent)
{
    QMouseEvent *event = (QMouseEvent*) vevent;
    if ((event->buttons() == Qt::RightButton)&&((m_selection == CREATEPOLYGON)||(m_selection == CREATEEVN)))
    {
        QPoint pointposition  = event->pos()/m_aspectratio  - m_ddpoint;

        m_polygoninconstruction.push_back(pointposition);
        repaint();
    }

    if ((event->buttons() == Qt::LeftButton)&&(m_selection == CREATEPOLYGON)&&(m_polygoninconstruction.size() > 2))
    {

        //Create poligon
        Polygon_2 polygon;
        for(int i = 0; i < m_polygoninconstruction.size(); i++){
            polygon.push_back(Point(m_polygoninconstruction.at(i).x(),m_polygoninconstruction.at(i).y()));
        }

        // always store points in clock wise orientation
        if(polygon.is_clockwise_oriented()){
            m_polygonset.push_back(polygon);
            m_polygoninconstruction.clear();
            repaint();
        }else{
            polygon.clear();
            for(int i = m_polygoninconstruction.size() -1; i >=0 ; i--){
                polygon.push_back(Point(m_polygoninconstruction.at(i).x(),m_polygoninconstruction.at(i).y()));
            }
            m_polygonset.push_back(polygon);
            m_polygoninconstruction.clear();
            repaint();
        }

    }
    /*

    if ((event->buttons() == Qt::LeftButton)&&(m_selection == CREATEEVN)&&(m_polygoninconstruction.size() > 2))
    {
        //Create poligon
        m_environment.clear();

        for(int i = 0; i < m_polygoninconstruction.size(); i++){
            m_environment.push_back(Point(m_polygoninconstruction.at(i).x(),m_polygoninconstruction.at(i).y()));
        }

        if(!m_environment.is_counterclockwise_oriented()){
            m_environment.clear();
            for(int i = m_polygoninconstruction.size() -1; i >= 0 ; i--){
                m_environment.push_back(Point(m_polygoninconstruction.at(i).x(),m_polygoninconstruction.at(i).y()));
            }
        }
        m_polygoninconstruction.clear();
        repaint();
    }*/

}

void Image::deleteClient(void *vevent)
{
    QMouseEvent *event = (QMouseEvent*) vevent;
    if ((event->buttons() == Qt::RightButton)&&(m_selection == DELETCLIENT)){
        for(int i = 0; i < m_clientset.size(); i++){
            QPoint pointposition  = event->pos()/m_aspectratio - m_ddpoint;
            QPoint distance = m_clientset.at(i) - pointposition;
            if((abs(distance.x()) < POLYGONSIZE*m_aspectratio) && (abs(distance.y()) < POLYGONSIZE*m_aspectratio)){
                m_clientset.remove(i);
                repaint();
                return;
            }
        }
    }
}


void Image::setSelection(Image::Selection selection)
{
    m_selection = selection;
    std::cout << m_selection << std::endl;

}

void Image::createTree(TREE tree){
    m_tree = tree;

    if(m_tree == GRAPHSTEINER){
        VisibilityGraph visibility;
        m_graphdata = visibility.createVisibilityGraph(&m_environment,&m_clientset,&m_polygonset);
        SteinerApproximation steiner;
        AdjList* newgraph = steiner.calculateSteinerTree(m_graphdata->graph,&m_graphdata->pointBag->getTerminals(),3);

        m_graphdata->graph = newgraph;

        std::cout << "Grath Tree Size " << m_graphdata->getTreeSize() << std::endl;
    }

    if(m_tree == EUCLIDEANSTEINER){
        m_graphdata = m_euclideansteiner->createSteinerGraph(&m_environment,&m_clientset,&m_polygonset);
        std::cout << "Euclidean Tree Size " << m_graphdata->getTreeSize() << std::endl;
    }



    repaint();

}

void Image::drawClients(void *)
{
    QPainter painter(this);
    for(int i = 0; i < m_clientset.size(); i++){
        QPoint drawposition = m_clientset.at(i)*m_aspectratio + m_ddpoint;
        painter.fillRect(QRect(drawposition-m_drawclientsize*m_aspectratio,drawposition+m_drawclientsize*m_aspectratio),QColor(0, 0, 0, 255));
    }
}

void Image::drawPolygons(void *)
{

    QPainter painter(this);
    //draw enviroment
    painter.setBrush(QBrush(Qt::blue,Qt::NoBrush));
    painter.setPen(Qt::blue);
    QPolygon polygontodraw;
    for(int j = 0; j < m_environment.size(); j++){
        QPoint drawposition = QPoint(m_environment[j].x(),m_environment[j].y())*m_aspectratio + m_ddpoint;
        polygontodraw.append(drawposition);
    }
    painter.drawPolygon(polygontodraw,Qt::OddEvenFill);


    //draw constructed polygons
    painter.setBrush(Qt::green);
    for(int i = 0; i < m_polygonset.size(); i++){
        QPolygon polygontodraw;
        Polygon_2 polygon = m_polygonset.at(i);
        for(int j = 0; j < polygon.size(); j++){
            QPoint drawposition = QPoint(polygon[j].x(),polygon[j].y())*m_aspectratio + m_ddpoint;
            polygontodraw.append(drawposition);
        }
        painter.drawPolygon(polygontodraw,Qt::WindingFill);
    }

    //draw polygon in construction
    for(int i = 0; i < m_polygoninconstruction.size(); i++){
        QPoint drawposition = m_polygoninconstruction.at(i)*m_aspectratio + m_ddpoint;
        if(m_selection == CREATEEVN)
            painter.fillRect(QRect(drawposition-m_drawpoligonsize,drawposition+m_drawpoligonsize),QColor(0, 0, 255, 255));
        else
            painter.fillRect(QRect(drawposition-m_drawpoligonsize,drawposition+m_drawpoligonsize),QColor(0, 255, 0, 255));

        if(i < m_polygoninconstruction.size() - 1){
            if(m_selection == CREATEEVN)
                painter.setPen(QPen(QColor(0, 0, 255), POLYGONSIZE, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin));
            else
                painter.setPen(QPen(QColor(0, 255, 0), POLYGONSIZE, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin));
            QPoint nextpoint = m_polygoninconstruction.at(i+1)*m_aspectratio + m_ddpoint;
            painter.drawLine (drawposition,nextpoint);
        }
    }



}

void Image::drawGraph(void *)
{
    QPainter painter(this);
    painter.setPen(QPen(QColor(0, 0, 255), POLYGONSIZE, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin));
    if(m_graphdata){
        //for each vertex
        for(int i = 0; i < m_graphdata->graph->size; i++){
            for(int j = 0; j < m_graphdata->graph->List[i]->size(); j++){
                GraphNode* node = m_graphdata->graph->List[i]->at(j);
                painter.drawLine(m_graphdata->pointBag->getdata(i)*m_aspectratio + m_ddpoint,m_graphdata->pointBag->getdata(node->j)*m_aspectratio + m_ddpoint);
            }
        }
    }
}

void Image::drawExternal(void *)
{

}

void Image::saveImage()
{

    QString fileName;
    if(m_tree == EUCLIDEANSTEINER)
        fileName.append("euclideanexperiment.png");
    else
        fileName.append("graphexperiment.png");

    QPixmap pixMap = QPixmap::grabWidget(this);
    pixMap.save(fileName);


}

void Image::createExperiment()
{
    QDir dir(EXPERIMENTDIR);
    dir.setFilter(QDir::Dirs);
    int n = dir.entryList().size()/2;

    QString euclideanexperiment;
    euclideanexperiment.append(QString::number(n)+QString("euclideanexperiment"));
    dir.mkdir(euclideanexperiment);

    QString graphexperiment;
    graphexperiment.append(QString::number(n)+QString("graphexperiment"));
    dir.mkdir(graphexperiment);


    QString graphexperimentdir(EXPERIMENTDIR+graphexperiment+QString("/"));
    QString euclideanexperimentdir(EXPERIMENTDIR+euclideanexperiment+QString("/"));

    VisibilityGraph visibility;
    VisibilityGraphData* graphdata = visibility.createVisibilityGraph(&m_environment,&m_clientset,&m_polygonset);
    SteinerApproximation steiner;
    AdjList* newgraph = steiner.calculateSteinerTree(graphdata->graph,&graphdata->pointBag->getTerminals(),3);

    graphdata->graph = newgraph;

    graphdata->saveFile1(graphexperimentdir.toStdString());
    graphdata->saveFile2(graphexperimentdir.toStdString());
    graphdata->saveFile3(graphexperimentdir.toStdString());


    graphdata = m_euclideansteiner->createSteinerGraph(&m_environment,&m_clientset,&m_polygonset);
    graphdata->saveFile1(euclideanexperimentdir.toStdString());
    graphdata->saveFile2(euclideanexperimentdir.toStdString());
    graphdata->saveFile3(euclideanexperimentdir.toStdString());
}

void Image::deletePolygon(void *vevent)
{
    QMouseEvent *event = (QMouseEvent*) vevent;
    if ((event->buttons() == Qt::RightButton)&&(m_selection == DELETEPOLYGON)){
        for(int i = 0; i < m_polygonset.size(); i++){
            Polygon_2 polygon = m_polygonset.at(i);

            QPoint pointposition = event->pos()/m_aspectratio  - m_ddpoint;
            Point p(pointposition.x(),pointposition.y());
            if(polygon.has_on_bounded_side(p)){
                m_polygonset.remove(i);
                repaint();
                return;
            }
        }
    }
}


