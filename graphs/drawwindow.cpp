#include "drawwindow.h"
#include "ui_drawwindow.h"

DrawWindow::DrawWindow(QWidget *parent) : QDialog(parent), ui(new Ui::DrawWindow) {
    ui->setupUi(this);

    colorList << "olivedrab"
              << "orange"
              << "orangered"
              << "orchid"
              << "palegoldenrod"
              << "palegreen"
              << "paleturquoise"
              << "palevioletred"
              << "papayawhip"
              << "peachpuff"
              << "peru"
              << "pink"
              << "plum"
              << "powderblue"
              << "purple"
              << "rosybrown"
              << "royalblue"
              << "saddlebrown"
              << "salmon"
              << "sandybrown"
              << "seagreen";
}

DrawWindow::~DrawWindow() {
    delete ui;
}

void DrawWindow::paintEvent(QPaintEvent *event) {
    ui->lineGraphDrawArea->update();
}

void DrawWindow::setLineGraph(Graph *lineGraph) {
    this->lineGraph_ = lineGraph;
    this->ui->lineGraphDrawArea->setGraph(lineGraph);
    for(int i = 0; i < lineGraph_->getNodeCount(); i++) {
        ui->nwList->addItem(QString::number(i));
        lineGraph->getNode(i)->setColor(colorList.at(i));
    }
}

void DrawWindow::updateLineGraph() {
#include <QRandomGenerator>
    for (int i = 0; i < lineGraph_->getNodeCount(); i++) {
        lineGraph_->getNode(i)->setHighlighted(false);
    }
    for (int i: iligra_->highlighted) {
        lineGraph_->getNode(i)->setHighlighted(true);
    }
}

void DrawWindow::setNodeGraph(Graph *nodeGraph) {
    this->nodeGraph_ = nodeGraph;
    this->ui->nodeGraphDrawArea->setGraph(nodeGraph);
}

void DrawWindow::updateNodeGraph(){

}

void DrawWindow::setCurrentNode(int currentNode) {
    ui->currentNodeLabel->setText(QString::number(currentNode));
}

void DrawWindow::on_nextStepButton_clicked()
{
    iligra_->changeState();
    setCurrentStepInfo(iligra_->stepInfo);
    updateLineGraph();
    updateNodeGraph();
    update();
}

void DrawWindow::setCurrentStepInfo(QString stepInfo) {
    ui->stepLabel->setText(stepInfo);
}

void DrawWindow::setIligra(Iligra *iligra) {
    iligra_ = iligra;
    setCurrentStepInfo(iligra_->stepInfo);
    setLineGraph(&(iligra_->G));
    setNodeGraph(&(iligra_->H));
    updateLineGraph();
}

void DrawWindow::on_loadFromFileButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,QString(),QString(), QString(),nullptr,
                                                    QFileDialog::DontUseNativeDialog);
    std::cout << fileName.toStdString() << std::endl;
    iligra_->loadFromFile(fileName);
    this->update();
}
