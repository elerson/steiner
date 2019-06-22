#ifndef EUCLIDEANSTEINER_H
#define EUCLIDEANSTEINER_H
#include <visilibity.hpp>
#include <visibilitygraph.h>
#include "image.h"

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef CGAL::Polygon_2<K> Polygon_2;

class EuclideanSteiner
{
public:
    EuclideanSteiner();
    VisibilityGraphData* createSteinerGraph(Polygon_2* enviroment, QVector<QPoint>* clientset,  QVector< Polygon_2 >* polygonset);
private:

    void generateVisibility(Polygon_2* enviroment, QVector<QPoint>* clientset,  QVector< Polygon_2 >* polygonset);
    void draw(QPainter& painter,QPoint displacement,double aspectratio);
    VisibilityGraphData* createGraph(QVector<QPoint> *clientset);

    bool isEnvironmentValid();
    bool areClientsValid();


    Polygon_2* m_enviroment;
    QVector<QPoint>* m_clientset;
    QVector< Polygon_2 >* m_polygonset;

    VisiLibity::Environment* m_mapenvironment;
    VisiLibity::Guards* m_mapclients;
    std::vector<VisiLibity::Polygon> m_polygons;
    std::vector<VisiLibity::Point> m_clients;

    struct cinfo *m_euclideantreeinfo;
    int* m_choosen;
    double m_epsilon;
};

class VisiLibityPointsGraph{
public:
    VisiLibityPointsGraph(int size);
    void add(VisiLibity::Polyline&,std::vector<VisiLibity::Point>&,bool);
    void printAll(std::string filename,int,std::vector<VisiLibity::Point>& points, std::vector<int>& index);


private:
    int m_size;
    bool** m_visibilitygraph;
    int** m_distancegraph;
};

#endif // EUCLIDEANSTEINER_H
