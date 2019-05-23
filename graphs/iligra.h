#ifndef ILIGRA_H
#define ILIGRA_H

#include <QString>

#include "graph.h"


class Iligra
{
public:
    Graph H;
    Graph G;

    Iligra();
    bool changeState();
    bool loadFromFile(QString file);

    enum State {NOT_READY, READY};

private:
    State state;
    bool changeState(State newState);
};

#endif // ILIGRA_H
