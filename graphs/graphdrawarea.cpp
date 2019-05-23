#include "graphdrawarea.h"
#include "ui_graphdrawarea.h"

GraphDrawArea::GraphDrawArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphDrawArea)
{
    ui->setupUi(this);
}

GraphDrawArea::~GraphDrawArea()
{
    delete ui;
}

void GraphDrawArea::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    if (graph_ != nullptr) {
        for(int nodeIterator = 0; nodeIterator < graph_->getNodeCount(); nodeIterator++) {
            QPoint nodePosition = graph_->getNodePosition(nodeIterator);
            std::vector<int> adjacent = graph_->getSingleAdjecencyList(nodeIterator);
            for (int adj: adjacent) {
                painter.drawLine(nodePosition, graph_->getNodePosition(adj));
            }
        }

        for(int nodeIterator = 0; nodeIterator < graph_->getNodeCount(); nodeIterator++) {
            QPoint nodePosition = graph_->getNodePosition(nodeIterator);

            painter.setBrush(Qt::red);
            painter.drawEllipse(nodePosition, RADIUS, RADIUS);
            painter.drawText(nodePosition, QString::number(nodeIterator));
        }
    }
}


void GraphDrawArea::setGraph(Graph *graph) {
    this->graph_ = graph;
}
