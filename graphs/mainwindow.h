#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QSpinBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void toogleBinaryButton();
    void changeNumberOfNodes(int i);

private:
    Ui::MainWindow *ui;

    static const int MAXNONODES              = 10;
    static const int NODESMATRIXBTNS_XSTART  = 250;
    static const int NODESMATRIXBTNS_YSTART  = 130;
    static const int NODESMATRIXBTNS_XSKIP   = 70;
    static const int NODESMATRIXBTNS_YSKIP   = 40;
    static const int NODESMATRIXBTNS_WIDTH   = 61;
    static const int NODESMATRIXBTNS_HEIGHT  = 31;

    QPushButton*** nodesMatrix;
    int noNodes;

    void addNodeLayerToMatrix(int layer);
    void addButtonToMatrix(int index_x, int index_y);
    void removeNodeLayerInMatrix(int layer);
    void removeButtonInMatrix(int index_x, int index_y);
};

#endif // MAINWINDOW_H
