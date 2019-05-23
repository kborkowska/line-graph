#ifndef GRAPHDRAWAREA_H
#define GRAPHDRAWAREA_H

#include <QWidget>
#include <QPainter>

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

private:
    Ui::GraphDrawArea *ui;
};

#endif // GRAPHDRAWAREA_H
