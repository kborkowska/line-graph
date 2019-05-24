#ifndef DRAWWINDOW_H
#define DRAWWINDOW_H

#include <QDialog>
#include "graph.h"
#include "iligra.h"
#include <QPainter>
#include <QFileDialog>
#include <iostream>

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
    void setLineGraph(Graph *lineGraph);
    void setNodeGraph(Graph *nodeGraph);
    void setCurrentNode(int currentNode);
    void setCurrentStepInfo(QString stepInfo);
    void setIligra(Iligra *iligra);
    void updateLineGraph();
    void updateNodeGraph();

private slots:
    void on_nextStepButton_clicked();

    void on_loadFromFileButton_clicked();

private:
    Ui::DrawWindow *ui;
    Graph *lineGraph_ = nullptr;
    Graph *nodeGraph_ = nullptr;
    int currentNode = 0;
    Iligra *iligra_;
    QStringList colorList;
};

#endif // DRAWWINDOW_H
