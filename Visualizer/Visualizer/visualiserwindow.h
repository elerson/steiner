#ifndef VISUALISERWINDOW_H
#define VISUALISERWINDOW_H

#include <QMainWindow>

namespace Ui {
class VisualiserWindow;
}

class VisualiserWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit VisualiserWindow(std::string file, QWidget *parent = 0);
    ~VisualiserWindow();
    void setImage(char*);
private slots:

    void changeAction();
    void createTree();
    void saveExperiment();
    void saveImage();
private:

    Ui::VisualiserWindow *ui;

};

#endif // VISUALISERWINDOW_H
