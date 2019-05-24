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

            painter.setBrush(QColor(graph_->getNode(nodeIterator)->getColor()));
            bool highlighted = graph_->getNode(nodeIterator)->isHighlighted();
            QPen pen(highlighted ? Qt::red : Qt::black, highlighted ? 5 : 1);
            painter.setPen(pen);
            painter.drawEllipse(nodePosition, RADIUS, RADIUS);

            painter.setPen(Qt::black);
            QString label = graph_->alphabeticalIndexing ? QString(QChar('A' + nodeIterator)) : QString::number(nodeIterator);
            painter.drawText(nodePosition.x() - RADIUS, nodePosition.y() - RADIUS, 2*RADIUS, 2*RADIUS, Qt::AlignHCenter | Qt::AlignVCenter, label);
        }
    }
}


void GraphDrawArea::setGraph(Graph *graph) {
    this->graph_ = graph;
}
