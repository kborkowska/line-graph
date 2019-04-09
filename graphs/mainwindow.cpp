#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGridLayout>
#include <QtGlobal>

void MainWindow::toogleBinaryButton(){
    QPushButton* ptrBtn = dynamic_cast<QPushButton*>(sender());
    ptrBtn->setText(QString::number((ptrBtn->text().toInt()+3)%2));
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

