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
    QRectF rectangle(10, 10, 10, 10);

    QPainter painter(this);
    painter.drawEllipse(rectangle);
}
