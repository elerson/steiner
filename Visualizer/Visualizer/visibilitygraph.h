#ifndef VISIBILITYGRAPH_H
#define VISIBILITYGRAPH_H
#include <visilibity.hpp>
#include "image.h"
#include "Graph.h"

#define COMPMINDIST 0.001

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef CGAL::Polygon_2<K> Polygon_2;

class PointsBag;

struct VisibilityGraphData{
    AdjList* graph;
    PointsBag* pointBag;   
    double getTreeSize();
    void saveFile1(std::string);
    void saveFile2(std::string);
    void saveFile3(std::string);
};

class VisibilityGraph
{
public:
    VisibilityGraph();
    VisibilityGraphData* createVisibilityGraph(Polygon_2* enviroment, QVector<QPoint>* clientset,  QVector< Polygon_2 >* polygonset);
private:

    bool isEnvironmentValid();
    bool areClientsValid();


    Polygon_2* m_enviroment;
    QVector<QPoint>* m_clientset;
    QVector< Polygon_2 >* m_polygonset;

    VisiLibity::Environment* m_mapenvironment;
    VisiLibity::Guards* m_mapclients;
    std::vector<VisiLibity::Polygon> m_polygons;
    std::vector<VisiLibity::Point> m_clients;

    double m_epsilon;

};

class PointsBag{
public:
    PointsBag();
    void add(VisiLibity::Polyline& polyline);
    void add(VisiLibity::Point& point);
    int takeIndex(VisiLibity::Point&);
    int getSize();
    double getDistance(int p1, int p2);
    QPoint getdata(int i);
    void generateTerminalPoints(QVector<QPoint>* clientset);
    std::vector<int>& getTerminals();
private:
    std::vector<VisiLibity::Point> m_bag;
    std::vector<int> m_terminals;
};

#endif // VISIBILITYGRAPH_H
