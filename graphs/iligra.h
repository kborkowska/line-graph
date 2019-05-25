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

    int nu;

    Iligra();
    bool changeState();
    bool loadFromFile(QString file);

    enum Step {DONE, NONE, LOADED, ARBITRARY_N1, ARBITRARY_N2, ADD_FIRST_NODES,
               CONNECT_N1, FIRST_CONECT_N2, N1_ONLY_NGBRS, ANALYSE_J, ONE_TWO_J,
               THREE_J, NU, INIT_SPECIAL, EACH_IN_J, SPECIAL, SPECIAL_ZERO_J, SPECIAL_ONE_J,
               SPECIAL_TWO_NS, SPECIAL_ZERO_JONE_L, SPECIAL_ZERO_JTWO_L, SPECIAL_ONE_JONE_L,
               SPECIAL_ONE_JTWO_L, SPECIAL_NS_IN_C, SPECIAL_NS_L,SPECIAL_TWO_NS_CLI,
               SPECIAL_TWO_NS_J, SPECIAL_TWO_NS_J_L, SPECIAL_TWO_NS_J_NU, SPECIAL_TWO_NS_CLI_J,
               SPECIAL_TWO_NS_CLI_JONE_L, SPECIAL_TWO_NS_CLI_JTWO_NR,
               SPECIAL_TWO_NS_CLI_JTWO_NR_L, SPECIAL_NX};

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
    int getZSize(int idx);
    int getNodeWithLessThanThreeZ();
    int getnr();
    int getns();
    int getnt();
    int getnx();
    bool checkIfIsNeighbour(int idx, int idxPot);
    void addNuConnectionToG(int i);
    bool isnxInC();
    bool isnsInC();
    void addToNh(int v, int n);
    bool isAClique(std::vector<int> potentialClique);
};

#endif // ILIGRA_H
