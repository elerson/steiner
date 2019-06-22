#include "visualiserwindow.h"
#include "ui_visualiserwindow.h"
#include <image.h>
#include <string>

VisualiserWindow::VisualiserWindow(std::string file, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VisualiserWindow)
{
    ui->setupUi(this);
    ui->Imagewidget->initialize(file);
    QObject::connect(ui->radiobtcreateclient,SIGNAL(clicked()),this,SLOT(changeAction()));
    //QObject::connect(ui->radiobtcreatepolygon,SIGNAL(clicked()),this,SLOT(changeAction()));
    QObject::connect(ui->radiobtdestroyclient,SIGNAL(clicked()),this,SLOT(changeAction()));
    //QObject::connect(ui->radiobtdestroypolygon,SIGNAL(clicked()),this,SLOT(changeAction()));
    //QObject::connect(ui->radiobtcreateenv,SIGNAL(clicked()),this,SLOT(changeAction()));
    //QObject::connect(ui->radiobtdestroyenv,SIGNAL(clicked()),this,SLOT(changeAction()));
    QObject::connect(ui->btcreatetree,SIGNAL(pressed()),this,SLOT(createTree()));
    QObject::connect(ui->btsave,SIGNAL(pressed()),this,SLOT(saveExperiment()));
    QObject::connect(ui->btsaveimg,SIGNAL(pressed()),this,SLOT(saveImage()));
}

void VisualiserWindow::setImage(char*){

}

VisualiserWindow::~VisualiserWindow()
{
    delete ui;
}

void VisualiserWindow::changeAction()
{
    if(ui->radiobtcreateclient->isChecked()){
        ui->Imagewidget->setSelection(Image::CREATECLIENT);
    }

   /* if(ui->radiobtcreatepolygon->isChecked()){
        ui->Imagewidget->setSelection(Image::CREATEPOLYGON);
    }*/

    if(ui->radiobtdestroyclient->isChecked()){
        ui->Imagewidget->setSelection(Image::DELETCLIENT);
    }

    /*if(ui->radiobtdestroypolygon->isChecked()){
        ui->Imagewidget->setSelection(Image::DELETEPOLYGON);
    }*/

    /*if(ui->radiobtcreateenv->isChecked()){
        ui->Imagewidget->setSelection(Image::CREATEEVN);
    }*/

    /*if(ui->radiobtdestroyenv->isChecked()){
        ui->Imagewidget->setSelection(Image::DELETEENV);
    }*/
}

void VisualiserWindow::createTree()
{
    if(ui->radiobtgraphsteiner->isChecked()){
        ui->Imagewidget->createTree(Image::GRAPHSTEINER);
    }

    if(ui->radiobteuclideansteiner->isChecked()){
        ui->Imagewidget->createTree(Image::EUCLIDEANSTEINER);
    }
}

void VisualiserWindow::saveExperiment()
{
     ui->Imagewidget->createExperiment();
}

void VisualiserWindow::saveImage()
{
     ui->Imagewidget->saveImage();
}
