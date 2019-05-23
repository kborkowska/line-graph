#ifndef DRAWWINDOW_H
#define DRAWWINDOW_H

#include <QDialog>
#include "graph.h"
#include <QPainter>

namespace Ui {
class DrawWindow;
}

class DrawWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DrawWindow(QWidget *parent = nullptr);
    ~DrawWindow();
    void paintEvent(QPaintEvent *event);
    void addLineGraph(Graph *lineGraph);
    void setCurrentNode(int currentNode);

private slots:
    void on_nextStepButton_clicked();

private:
    Ui::DrawWindow *ui;
    Graph *lineGraph_ = nullptr;
    int currentNode = 0;
};

#endif // DRAWWINDOW_H
