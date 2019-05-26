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

        for (int lineIterator = 0; lineIterator < graph_->getLineCount(); lineIterator++) {
            QFont font = painter.font();
            font.setPointSize(10);
            painter.setFont(font);
            Graph::Line *line = graph_->lines_[lineIterator].get();
            painter.setPen(Qt::black);
            if (line->label1 != -1){
                QPen pen(QColor(graph_->colorList.at(line->label1)), 2);
                painter.setPen(pen);
            }
            QPoint p1, p2;

            if(line->node1 >= 0 && line->node2 == -10) {
                p1 = graph_->getNodePosition(line->node1);
                QRandomGenerator gen(line->node1 + line->label1 + line->label2 + 1);
                int dir = gen.bounded(0, 2*3.14);
                p2 = QPoint(p1.x() + 80*sin(dir), p1.y() +80*cos(dir));
            } else if(line->node2 >= 0 && line->node1 == -10) {
                p1 = graph_->getNodePosition(line->node2);
                QRandomGenerator gen(line->node2 + line->label1 + line->label2 + 1);
                int dir = gen.bounded(0, 2*3.14);
                p2 = QPoint(p1.x() + 80*sin(dir), p1.y() +80*cos(dir));
            } else {
                p1 = graph_->getNodePosition(line->node1);
                p2 = graph_->getNodePosition(line->node2);
            }

            painter.drawLine(p1, p2);
            QLine qline(p1, p2);
            QPoint position = p1 + QPoint(qline.dx()/2, qline.dy()/2);
            if (line->label1 != -1){
                painter.setPen(QColor(graph_->colorList.at(line->label1)));
                painter.drawText(position.x() - 5, position.y(), QString::number(line->label1));
            }
            if (line->label2 != -1) {
                painter.setPen(QColor(graph_->colorList.at(line->label2)));
                painter.drawText(position.x() + 5, position.y(), QString::number(line->label2));
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
