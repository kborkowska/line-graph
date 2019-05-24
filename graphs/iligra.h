#ifndef ILIGRA_H
#define ILIGRA_H

#include <QString>

#include "graph.h"


class Iligra
{
public:
    Graph H;
    Graph G;

    std::vector<int> Nw; //The set of the nodes in H,
    //corresponding to thelinks inGwhose incident nodes are not yet determined
    std::vector<int> Nh; //The set of the nodes inH,
    //corresponding to thelinks inGof which one incident node is determined
    std::vector<int> Nb; //The set of the neighbors
    std::vector<int> J;

    std::vector<int> vlh;
    std::vector<int> highlighted;

    QString stepInfo;

    Iligra();
    bool changeState();
    bool loadFromFile(QString file);

    enum Step {NONE, LOADED, ARBITRARY_N1, ARBITRARY_N2, ADD_FIRST_NODES,
               CONNECT_N1, FIRST_CONECT_N2, N1_ONLY_NGBRS, ANALYSE_J, ONE_TWO_J,
               THREE_J, NU, INIT_SPECIAL, EACH_IN_J, SPECIAL};

private:
    Step step;
    bool changeStep(Step newStep);
    int choseArbitraryNode();
    int choseArbitraryNeighbour();
    void addFirstNodes();
    void connectN1();
    void firstConnectN2();
    void n1OnlyNeighbours();
    int rightNExistInJ_oneTwo();
    int rightNExistInJ_three();
    void setNu(int nu);
    int getNodeWithLessThanThreeZ();
};

#endif // ILIGRA_H
