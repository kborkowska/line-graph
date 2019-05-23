#ifndef GRAPHDRAWAREA_H
#define GRAPHDRAWAREA_H

#include <QWidget>
#include <QPainter>
#include <QRandomGenerator>
#include "graph.h"

#define RADIUS 15

namespace Ui {
class GraphDrawArea;
}

class GraphDrawArea : public QWidget
{
    Q_OBJECT

public:
    explicit GraphDrawArea(QWidget *parent = nullptr);
    ~GraphDrawArea();
    void paintEvent(QPaintEvent *event);
    void setGraph(Graph *graph);

private:
    Ui::GraphDrawArea *ui;
    Graph *graph_ = nullptr;
};

#endif // GRAPHDRAWAREA_H
