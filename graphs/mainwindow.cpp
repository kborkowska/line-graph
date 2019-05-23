#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGridLayout>
#include <QtGlobal>

void MainWindow::toogleBinaryButton(){
    QPushButton* ptrBtn = dynamic_cast<QPushButton*>(sender());
    //ptrBtn->setText(QString::number((ptrBtn->text().toInt()+3)%2));
    int idx_x,idx_y;
    for(int i = 0; i < MAXNONODES; ++i){
        for(int j = 0; j < MAXNONODES; ++j){
            if(nodesMatrix[i][j] == ptrBtn){
                idx_x = i;
                idx_y = j;
                break;
            }
        }
    }
    if(idx_x!= idx_y){
        nodesMatrix[idx_x][idx_y]->setText(QString::number((nodesMatrix[idx_x][idx_y]->text().toInt()+3)%2));
        nodesMatrix[idx_y][idx_x]->setText(QString::number((nodesMatrix[idx_y][idx_x]->text().toInt()+3)%2));
    }
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    nodesMatrix = new QPushButton**[MAXNONODES];
        for (int i = 0; i < MAXNONODES; i++) {
            nodesMatrix[i] = new QPushButton*[MAXNONODES]();
        }

    ui->setupUi(this);

    noNodes = 2;

    addNodeLayerToMatrix(1);
    addNodeLayerToMatrix(2);

    connect(ui->spinBox, QOverload<int>::of(&QSpinBox::valueChanged),this, [=](int i) {changeNumberOfNodes(i); });

}

MainWindow::~MainWindow()
{
    delete ui; 
}


void MainWindow::addNodeLayerToMatrix(int layer)
{
    addButtonToMatrix(layer-1, layer-1);
    for(int i = layer-2; i > -1; --i){
        addButtonToMatrix(layer-1, i);
        addButtonToMatrix(i, layer-1);
    }
}

void MainWindow::removeNodeLayerInMatrix(int layer)
{
    removeButtonInMatrix(layer-1, layer-1);
    for(int i = layer-2; i > -1; --i){
        removeButtonInMatrix(layer-1, i);
        removeButtonInMatrix(i, layer-1);
    }
}

void MainWindow::removeButtonInMatrix(int index_x, int index_y){
    nodesMatrix[index_x][index_y]->deleteLater();
    nodesMatrix[index_x][index_y] = nullptr;
}

void MainWindow::addButtonToMatrix(int index_x, int index_y){
    nodesMatrix[index_x][index_y] = new QPushButton(ui->centralWidget);
    nodesMatrix[index_x][index_y]->setGeometry(NODESMATRIXBTNS_XSTART + NODESMATRIXBTNS_XSKIP*(index_x),
                                               NODESMATRIXBTNS_YSTART + NODESMATRIXBTNS_YSKIP*(index_y),
                                               NODESMATRIXBTNS_WIDTH, NODESMATRIXBTNS_HEIGHT);
    nodesMatrix[index_x][index_y]->setText("0");
    nodesMatrix[index_x][index_y]->show();
    connect(nodesMatrix[index_x][index_y], SIGNAL (clicked()),this, SLOT (toogleBinaryButton()));
}


void MainWindow::changeNumberOfNodes(int i){
    if(noNodes < i && i <= MAXNONODES){
        addNodeLayerToMatrix(i);
    } else if (noNodes > i){
        removeNodeLayerInMatrix(noNodes);
    }
    noNodes = i;
}


void MainWindow::on_pushButton_101_clicked()
{
    drawWindow = new DrawWindow();
    Graph *graph = new Graph();
    graph->addNode();
    graph->addNode();
    graph->addNode();
    graph->addNode();
    graph->addNode();
    graph->connect(0, 1);
    graph->connect(0, 2);
    graph->connect(2, 3);
    graph->connect(1, 2);
    graph->connect(1, 3);
    graph->connect(0, 4);
    graph->connect(4, 3);

    graph->repositionNodes();

    drawWindow->addLineGraph(graph);
    drawWindow->show();
    drawWindow->update();
}
