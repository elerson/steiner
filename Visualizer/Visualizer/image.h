#ifndef IMAGE_H
#define IMAGE_H

#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QDragEnterEvent>
#include <QApplication>
#include <QVector>
#include <QWheelEvent>
#include <QPaintDevice>
#include <QPointer>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <string>


#define SENSIBILITY 0.1
#define CLIENTSIZE 10
#define POLYGONSIZE 5

#include <CGAL/Polygon_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#define EXPERIMENTDIR "experiments/"

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef CGAL::Polygon_2<K> Polygon_2;

class EuclideanSteiner;

struct VisibilityGraphData;

class Image : public QWidget
{
    Q_OBJECT
public:
    enum Selection { CREATECLIENT, DELETCLIENT,CREATEPOLYGON,DELETEPOLYGON,CREATEEVN,DELETEENV};
    enum TREE { EUCLIDEANSTEINER, GRAPHSTEINER};
    explicit Image(QWidget *parent = 0);
    void initialize(std::string image_file);
    
signals:

public slots:

protected:
    virtual void paintEvent ( QPaintEvent * event );
    virtual void dragEnterEvent ( QDragEnterEvent * event );
    virtual void dragMoveEvent ( QDragMoveEvent * event );
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent ( QMouseEvent * event );
    virtual void wheelEvent ( QWheelEvent * event );
    virtual void keyPressEvent ( QKeyEvent * event );

    void drawImage(void *);
    void drawClients(void *);
    void drawPolygons(void *);
    void drawGraph(void *);
    void drawExternal(void *);



    void dragPicture(void * event);
    void movePicture(void * event);
    void dropPicture(void * event);

    void createClient(void *vevent);
    void createPolygon(void *vevent);
    void deleteClient(void *vevent);
    void deletePolygon(void *vevent);
    void createEnvironment();
    void createObstacles();

public:
    void saveImage();
    void createExperiment();
    void setSelection(Selection selection);
    void createTree(TREE tree);


private:
    typedef void (Image::*Functionpointer)( void *) ;

    std::string image_file;
    QImage m_image;
    QPoint m_ddpoint; //drag and drop point
    QPoint m_dragpos;
    QPoint m_dragposzoom;
    QPoint m_dragddpoint;
    QPoint m_maxdiff;
    QPoint m_zoom;
    QPoint m_drawclientsize;
    QPoint m_drawpoligonsize;

    QVector<Functionpointer> m_mousedragfunctions;
    QVector<Functionpointer> m_mousemovefunctions;
    QVector<Functionpointer> m_mousedropfunctions;
    QVector<Functionpointer> m_drawfunctions;


    QVector<QPoint> m_clientset;
    QVector<QPoint> m_polygoninconstruction;

    QVector< Polygon_2 > m_polygonset;
    Polygon_2 m_environment;

    Selection m_selection;

    float m_aspectratio;
    cv::Mat cv_img;

    VisibilityGraphData* m_graphdata;

    EuclideanSteiner* m_euclideansteiner;

    TREE m_tree;
};

#endif // IMAGE_H
