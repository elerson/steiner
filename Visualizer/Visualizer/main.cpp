#include <QtGui/QApplication>
#include "visualiserwindow.h"
#include <string>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    std::string file(argv[1]);
    VisualiserWindow w(file);
    w.show();
    
    return a.exec();
}
