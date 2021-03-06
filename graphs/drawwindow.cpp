#include "drawwindow.h"
#include "ui_drawwindow.h"

DrawWindow::DrawWindow(QWidget *parent) : QDialog(parent), ui(new Ui::DrawWindow) {
    ui->setupUi(this);
}

DrawWindow::~DrawWindow() {
    delete ui;
}

void DrawWindow::paintEvent(QPaintEvent *event) {
    ui->lineGraphDrawArea->update();
    ui->nodeGraphDrawArea->update();
}

void DrawWindow::setLineGraph(Graph *lineGraph) {
    this->lineGraph_ = lineGraph;
    this->ui->lineGraphDrawArea->setGraph(lineGraph);
}

void DrawWindow::updateLineGraph() {
#include <QRandomGenerator>
    for (int i = 0; i < lineGraph_->getNodeCount(); i++) {
        lineGraph_->getNode(i)->setHighlighted(false);
    }
    for (int i: iligra_->highlighted) {
        lineGraph_->getNode(i)->setHighlighted(true);
    }

    lineGraph_->colorNodes();
}

void DrawWindow::setNodeGraph(Graph *nodeGraph) {
    this->nodeGraph_ = nodeGraph;
    this->ui->nodeGraphDrawArea->setGraph(nodeGraph);
}

void DrawWindow::updateNodeGraph(){

}

void DrawWindow::setCurrentNode(int currentNode) {

}

void DrawWindow::on_nextStepButton_clicked()
{
    iligra_->changeState();
    setCurrentStepInfo(iligra_->stepInfo);
    updateLineGraph();
    updateNodeGraph();
    updateDrawWindow();
    update();
}

void DrawWindow::setCurrentStepInfo(QString stepInfo) {
    ui->stepLabel->setText(stepInfo);
}

void DrawWindow::setIligra(Iligra *iligra) {
    iligra_ = iligra;
    setCurrentStepInfo(iligra_->stepInfo);
    setLineGraph(&(iligra_->H));
    setNodeGraph(&(iligra_->G));
    iligra_->G.alphabeticalIndexing = true;
    updateLineGraph();
}

void DrawWindow::on_loadFromFileButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,QString(),QString(), QString(),nullptr,
                                                    QFileDialog::DontUseNativeDialog);
    delete (iligra_);
    iligra_ = new Iligra();
    iligra_->loadFromFile(fileName);
    setLineGraph(&(iligra_->H));
    setNodeGraph(&(iligra_->G));
    lineGraph_->repositionNodes();
    lineGraph_->fillLines();
    updateDrawWindow();
    this->update();
}

void DrawWindow::updateDrawWindow() {
    ui->NhList->clear();
    ui->NwList->clear();
    ui->JList->clear();
    ui->CList->clear();

    for(auto it = iligra_->Nh.begin(); it != iligra_->Nh.end(); it++) {
        ui->NhList->addItem(QString::number(*it));
    }

    for(auto it = iligra_->Nw.begin(); it != iligra_->Nw.end(); it++) {
        ui->NwList->addItem(QString::number(*it));
    }
    for(auto it = iligra_->J.begin(); it != iligra_->J.end(); it++) {
        ui->JList->addItem(QString::number(*it));
    }
    for(auto it = iligra_->C.begin(); it != iligra_->C.end(); it++) {
        ui->CList->addItem(QString::number(*it));
    }

    lineGraph_->repositionNodes();
    nodeGraph_->repositionNodes();
    updateLineGraph();
    updateNodeGraph();
    update();
}
