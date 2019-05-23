#ifndef ILIGRA_H
#define ILIGRA_H

#include <QString>

#include "graph.h"


class Iligra
{
public:
    Graph H;
    Graph G;

    std::vector<int> Nw;
    std::vector<int> Nh;
    std::vector<int> Nb;

    std::vector<int> highlighted;

    QString stepInfo;

    Iligra();
    bool changeState();
    bool loadFromFile(QString file);

    enum Step {NONE, LOADED, ARBITRARY_N1, ARBITRARY_N2};

private:
    Step step;
    bool changeStep(Step newStep);
    int choseArbitraryNode();
    int choseArbitraryNeighbour();
};

#endif // ILIGRA_H
