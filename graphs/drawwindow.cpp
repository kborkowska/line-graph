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
}

void DrawWindow::addLineGraph(Graph *lineGraph) {
    this->lineGraph_ = lineGraph;
    this->ui->lineGraphDrawArea->setGraph(lineGraph);
    for(int i = 0; i < lineGraph_->getNodeCount(); i++) {
        ui->nwList->addItem(QString::number(i));
    }
}

void DrawWindow::setCurrentNode(int currentNode) {
    ui->currentNodeLabel->setText(QString::number(currentNode));
}

void DrawWindow::on_nextStepButton_clicked()
{
    currentNode++;
    setCurrentNode(currentNode);
}
