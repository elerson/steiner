#include <QtGui/QApplication>
#include "visualiserwindow.h"
#include <string>
#include <iostream>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    std::string file(argv[1]);
    std::string file2("");

    if(argc == 3){
      file2 = std::string(argv[2]);
    }

    VisualiserWindow w(file, file2);
    w.show();


    
    return a.exec();
}
