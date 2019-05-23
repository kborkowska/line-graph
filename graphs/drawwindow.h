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

private:
    Ui::DrawWindow *ui;
    Graph *lineGraph;
};

#endif // DRAWWINDOW_H
