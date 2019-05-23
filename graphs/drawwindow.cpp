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
}
