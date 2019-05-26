#include "iligra.h"

#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <iostream>

Iligra::Iligra(){
    step = NONE;
    stepInfo = "Load a line graph";
    std::cout<<stepInfo.toStdString()<<std::endl;
    nu = -1;
}


bool Iligra::changeState(){

    switch(step){
          case NONE:            stepInfo = "Can't move to the next step!\n Load a line graph";
                                return false;
          case LOADED:          step = ARBITRARY_N1;
                                stepInfo = "Chose one arbitrary node - n1";
                                choseArbitraryNode();
                                return true;
          case ARBITRARY_N1:    stepInfo = "Chose one of its neighboours - n2";
                                step = ARBITRARY_N2;
                                choseArbitraryNeighbour();
                                return true;
          case ARBITRARY_N2:    stepInfo = "Add two nodes to the reconstructed graph G.";
                                step = ADD_FIRST_NODES;
                                addFirstNodes();
                                return true;
          case ADD_FIRST_NODES: stepInfo = "Add a link between new nodes G. It corresponds to n1. "
                                           "Remove n1 from Nw";
                                step = CONNECT_N1;
                                connectN1();
                                return true;
          case CONNECT_N1:      stepInfo = "To the first node v1 in G add a verticle corresponding to n2. "
                                           "Move n2 form Nw to Nh";
                                step = FIRST_CONECT_N2;
                                firstConnectN2();
                                return true;
          case FIRST_CONECT_N2: stepInfo = "Set all neighbours of n1, but not n2, as incident to v2."
                                           "Set J to all the nodes that are both neighbours to n1 and n2";
                                step = N1_ONLY_NGBRS;
                                n1OnlyNeighbours();
                                return true;
          case N1_ONLY_NGBRS:   stepInfo = "Analyze size of J.";
                                step = ANALYSE_J;
                                return true;
          case ANALYSE_J:       {if(J.size()==0){
                                 stepInfo = "ERROR!";
                                 step = DONE;
                                 return false;
                                 }else if(J.size()==1 || J.size()==2){
                                    stepInfo = "J has 1 or 2 nodes. Check whether one of its nodes (nu) has more than 2 neighbours that are not n1 or n1"
                                               " and all of its neighbours are connected to either n1 or n2.";
                                    step = ONE_TWO_J;
                                } else {
                                    stepInfo = "J has 3 or more nodes. Check whether there is a node nu"
                                               " that is not adjecent to any other node in J";
                                    step = THREE_J;
                                }
                                return true;
                                }
          case ONE_TWO_J:       {nu = rightNExistInJ_oneTwo();
                                 if(nu>=0){
                                    stepInfo = "Found right nu. Remove it form J and Nw, add to Nh."
                                               "Set nu as link of v2";
                                    step = EACH_IN_J;
                                    addNuConnectionToG(G.getNodesIndexes()[1]);
                                 } else {
                                     stepInfo = "Haven't found right nu, commencing special cases scenario";
                                     step = INIT_SPECIAL;

                                 }
                                 return true;
                                }
           case THREE_J:        {nu = rightNExistInJ_three();
                                 if(nu>=0){
                                     stepInfo = "Found right nu. Remove it form J and Nw, add to Nh."
                                                "Set nu as link of v2";
                                     addNuConnectionToG(nu);
                                 } else {
                                     stepInfo = "Haven't found right nu. Moving on.";
                                 }
                                 step = EACH_IN_J;
                                 return true;
                                }
           case INIT_SPECIAL:   {nu = getNodeWithLessThanThreeZ();
                                 highlighted.push_back(nu);
                                 if(nu>=0){
                                     stepInfo = "Found nu for special cases: " + QString::number(nu);
                                     step = SPECIAL;
                                 } else {
                                     stepInfo = "No proper candidate for special cases scenario. Moving on.";
                                     step = EACH_IN_J;
                                 }
                                 return true;
                                }
           case SPECIAL:        {int zSize = getZSize(nu);
                                 if(zSize == 0){
                                     stepInfo = "nu has no neighbours that are not n1 or n2."
                                                " Again check size of J";
                                     step = SPECIAL_ZERO_J;
                                 } else if(zSize == 1){
                                     stepInfo = "nu has one neighbour that is not n1 or n2."
                                                " Again check size of J";
                                     step = SPECIAL_ONE_J;
                                 } else if(zSize == 2){
                                     stepInfo = "nu has two neighbours that are not n1 or n2."
                                                " Checking whether ns belongs ";
                                     step = SPECIAL_TWO_NS;
                                 }
                                 return true;
                                }
           case SPECIAL_ZERO_J: {if(J.size()==1){
                                    stepInfo = "J nas one member. Check the number of links in G - L.";
                                    step = SPECIAL_ZERO_JONE_L;
                                 } else if(J.size()==2 && checkIfIsNeighbour(nu, getnr())){
                                    stepInfo = "J nas two members and nr is not a part of nu's neighbourhood. "
                                               "Check the number of links in G - L.";
                                    step = SPECIAL_ZERO_JTWO_L;
                                 }else{
                                    stepInfo = "nr belongs to neighbouthood of nu. Moving on.";
                                    step = EACH_IN_J;
                                 }
                                 return true;
                                }
            case SPECIAL_ZERO_JONE_L:
                if(H.getNodeCount()==3){
                    stepInfo = "G is K_3 or K_1,3";
                    step = DONE;
                    G.connect(1,2);
                } else if (H.getNodeCount()>3){
                    addNuConnectionToG(0);
                    stepInfo = "nu is connected to v1";
                    step = EACH_IN_J;
                } else {
                    stepInfo = "ERROR!";
                    step = DONE;
                    return false;
                }
                return true;
            case SPECIAL_ZERO_JTWO_L:
                if(H.getNodeCount()==4){
                    addNuConnectionToG(0);
                    stepInfo = "nu is connected to v1 (or v2 - isomorphic)";
                    step = EACH_IN_J;
                } else if (H.getNodeCount()>5){
                    int v = -1;
                    isnxInC() ? v=0:v=1;
                    addNuConnectionToG(v);
                    stepInfo = "nu is connected to v"+ QString::number(v);
                    step = EACH_IN_J;
                } else {
                    stepInfo = "ERROR!";
                    step = DONE;
                    return false;
                }
                return true;
            case SPECIAL_ONE_J:
                {if(J.size()==1){
                    stepInfo = "J nas one member. Check number of verticles in G - L.";
                    step = SPECIAL_ZERO_JTWO_L;
                } else if(J.size()==2 && checkIfIsNeighbour(nu, getnr())){
                    stepInfo = "J nas two members and nr is not a part of nu's neighbourhood. "
                               "Check ns's neighbourhood.";
                    step = SPECIAL_ONE_JTWO_L;
                }else{
                    stepInfo = "nr belongs to neighbouthood of nu. Moving on.";
                    step = EACH_IN_J;
                }
                return true;}
            case SPECIAL_ONE_JTWO_L:{
                if(!checkIfIsNeighbour(getnr(),getns())){
                    stepInfo = "ns doesn't belong to the neighbouthood of nr. "
                               "Analyse if ns belongs to C (sum of n1's and n2's neighbourhoods).";
                    step = SPECIAL_NS_IN_C;
                } else {
                    stepInfo = "ns belongs to the neighbouthood of nr. "
                               "Analyse L.";
                    step = SPECIAL_NS_L;
                }
                return true;}
           case SPECIAL_NS_IN_C:{
                if(isnsInC()){
                    addNuConnectionToG(G.getNodesIndexes()[0]);
                    stepInfo = "ns is part of C, hence nu's link will be connected to v1";
                } else {
                    addNuConnectionToG(G.getNodesIndexes()[1]);
                    stepInfo = "ns is not a part of C, hence nu's link will be connected to v2";
                }
                step = EACH_IN_J;
                return true;}
           case SPECIAL_NS_L:{
                int L = H.getNodeCount();
                if(L<4){
                    stepInfo = "ERROR!";
                    step = DONE;
                    return false;
                }else if(L == 5){
                    addNuConnectionToG(G.getNodesIndexes()[0]);
                    stepInfo = "nu is connected to v1 (or v2 - isomorphic)";
                    step = EACH_IN_J;
                } else {
                    int v = -1;
                    isnxInC() ? v=0:v=1;
                    addNuConnectionToG(G.getNodesIndexes()[v]);
                    stepInfo = "nu is connected to v"+ QString::number(v);
                    step = EACH_IN_J;
                }
                return true;}
           case SPECIAL_TWO_NS:{
                if(checkIfIsNeighbour(getnt(), getns())){
                    stepInfo = "ns is a neighbour od nt. Checking if ns, nt and n1 or n2 are a clique.";
                    step = SPECIAL_TWO_NS_CLI;
                } else {
                    stepInfo = "ns is not a neighbour od nt. Checking the size of J.";
                    step = SPECIAL_TWO_NS_J;
                }
                return true;}
           case SPECIAL_TWO_NS_J:{
                if(J.size()==1){
                    stepInfo = "J has only one node, hence nu is a link of v2";
                    addNuConnectionToG(G.getNodesIndexes()[1]);
                    step = EACH_IN_J;
                } else if(J.size()==2){
                    stepInfo = "J has two nodes, check if nr (node in J that is not nu) "
                               "is a neighbour of nu.";
                    step = SPECIAL_TWO_NS_J_NU;
                }
                return true;}
           case SPECIAL_TWO_NS_J_NU:{
                if(!checkIfIsNeighbour(nu,getnr())){
                    stepInfo = "nr is not a neighbour of nu, hence nu is a link of v2";
                    addNuConnectionToG(G.getNodesIndexes()[1]);
                } else {
                    stepInfo = "nr is a neighbour of nu, hence no connection is established. "
                               "Moving on";
                }
                step = EACH_IN_J;
                return true;}
           case SPECIAL_TWO_NS_CLI:{
                std::vector<int> cliOne = {H.getNodesIndexes()[0], getns(), getnt()};
                std::vector<int> cliTwo = {H.getNodesIndexes()[1], getns(), getnt()};
                if((isAClique(cliOne)||isAClique(cliTwo))){
                    stepInfo = "ns, nt and n1 or n2 form a clique, hence nu is a link of v2";
                    addNuConnectionToG(G.getNodesIndexes()[1]);
                    step = EACH_IN_J;
                } else {
                    stepInfo = "ns, nt and n1 or n2 do not form a clique. "
                               "Checking size of J";
                    step = SPECIAL_TWO_NS_CLI_J;
                }
                return true;}
           case SPECIAL_TWO_NS_CLI_J:{
                if(J.size()==1){
                    stepInfo = "Only nu is in J. Checking L - the number of links in G";
                    step = SPECIAL_TWO_NS_CLI_JONE_L;
                } else if(J.size()==2){
                    stepInfo = "Two nodes are in J. Checking in nr (other node in J) is connected to nu";
                    step = SPECIAL_TWO_NS_CLI_JTWO_NR;
                }
                return true;}
           case SPECIAL_TWO_NS_CLI_JONE_L:{
                if(H.getNodeCount()<5){
                    stepInfo = "L is too small - moving on;";
                    step = EACH_IN_J;
                } else if(H.getNodeCount()==5){
                    stepInfo = "L is equal 5, hence nu is a link of v2";
                    step = EACH_IN_J;
                    addNuConnectionToG(G.getNodesIndexes()[0]);
                } else {
                     stepInfo = "L is bigger than 5, hence checking if nx (a node in H, that is not "
                                "n1, n2, nu or nr) is not connected to n1, nor n2.";
                     step = SPECIAL_NX;
                }
                return true;}
           case SPECIAL_NX:{
                int nx = getnx();
                if(!(checkIfIsNeighbour(H.getNodesIndexes()[0],nx) &&
                     checkIfIsNeighbour(H.getNodesIndexes()[1],nx))){
                    stepInfo = "nx is not connected to neither n1 nor n2, "
                               "hence nu is connected to v2.";
                    addNuConnectionToG(G.getNodesIndexes()[1]);
                    step = EACH_IN_J;
                } else {
                    stepInfo = "nx is connected to either n1 or n2, "
                               "hence nu's connection cannot be established. Moving on.";
                }
                step = EACH_IN_J;
                return true;}
           case SPECIAL_TWO_NS_CLI_JTWO_NR:{
                if(checkIfIsNeighbour(nu, getnr())){
                    stepInfo = "nr is a naighbour of nu, "
                               "hence no connectin for nu is established. "
                               "Moving on.";
                    step = EACH_IN_J;
                } else {
                    stepInfo = "nr is not a neighbour of nu, hence checking L -"
                               " the number of links in G";
                    step = SPECIAL_TWO_NS_CLI_JTWO_NR_L;
                } return true;}
           case SPECIAL_TWO_NS_CLI_JTWO_NR_L:{
                if(H.getNodeCount()>6){
                    stepInfo = "G has less than 6 links - nothing can be establised for nu."
                               "Moving on.";
                    step = EACH_IN_J;
                } else if(H.getNodeCount()==6){
                    stepInfo = "G has 6 links - nu is a link of v2.";
                    step = EACH_IN_J;
                    addNuConnectionToG(G.getNodesIndexes()[1]);
                } else {
                    stepInfo = "G has more than 6 links - checking if "
                               "nx (a node in H, that is not "
                               "n1, n2, nu or nr) is not connected to n1, nor n2.";
                    step = SPECIAL_NX;
                } return true;}
           case EACH_IN_J:
                stepInfo="Connect all remaining nodes in J to v1. "
                         "Then check if H is a line graph.";
                connectAllJToV1();
                step = CHECK_IF_LINE_J;
                return true;
           case CHECK_IF_LINE_J:
                if(J.size()!=0){
                    stepInfo="J is not empty, hence checking if it forms a clique.";
                    step = CHECK_IF_LINE_J_CLI;
                } else {
                    stepInfo = "J is empty, checking the second condition for H being "
                               "a line graph.";
                    step = CHECK_IF_LINE_NB;
                }
                return true;
           case CHECK_IF_LINE_J_CLI:
                if(isAClique(J)){
                    stepInfo="J is a clique, hence checking the next condition.";
                    step = CHECK_IF_LINE_NB;
                } else {
                    stepInfo="J isn't a clique, and hence not a line graph. Finished processing.";
                    step = DONE;
                } return true;
           case CHECK_IF_LINE_NB:
                if(getNeighboursN1WithoutJ().size() != 0){
                    stepInfo="There are some nodes that are connected to n1 but are not in J. "
                             "Checking if this set of nodes is a clique";
                    step = CHECK_IF_LINE_NB_CLI;
                } else {
                    stepInfo="There aren't any nodes that are connected to n1 but are not in J. "
                             "For now is seem that H is a line graph. Moving on.";
                    step = WHILE_NH;
                } return true;
           case CHECK_IF_LINE_NB_CLI:
                if(isAClique(getNeighboursN1WithoutJ())){
                    stepInfo="Neighbourhood of n1 without J is a clique "
                             "For now is seem that H is a line graph. Moving on.";
                    step = CHECK_IF_LINE_NB;
                } else {
                    stepInfo="Neighbourhood of n1 without J isn't a clique, and hence not a line graph. Finished processing.";
                    step = DONE;
                } return true;
           case WHILE_NH:
                n=Nh[0];
                vl = vlh[0];
                checedkInN = 0;
                stepInfo:"As long as Nh is not empty go back to this step. "
                         "Take an arbitrary node form Nh. Node "+QString::number(n)+" was chosen";
                highlighted.push_back(n);
                step = ADD_V_AND_CONNECT;
                return true;
           case ADD_V_AND_CONNECT:
                stepInfo="Create a new verticle in G and connect it to the node link "
                            +QString::number(n)+"was connected to. "
                             "Remove the node form Nh and set C to epmty set.";
                G.addNode();
                connectFromNhToSecondNode(n, G.getNodesIndexes()[G.getNodeCount()-1]);
                C.clear();
                step = EACH_NR_IN_NB;
                return true;
           case EACH_NR_IN_NB:
                stepInfo = "If node "+QString::number(n)+" has any neighbours "
                           "in Nh or Nw take one. ";
                if(H.getSingleAdjecencyList(n).size()==0){
                    stepInfo += "No nodes in neighbourhood. Check if Nh is empty.";
                    step = IS_NH_EMPTY;
                } else {
                    for(int i = checedkInN ; i<H.getSingleAdjecencyList(n).size(); ++i){
                        ++checedkInN;
                        if(std::find(Nh.begin(), Nh.end(), H.getSingleAdjecencyList(n)[i]) != Nh.end()){
                            nn = H.getSingleAdjecencyList(n)[i];
                            stepInfo+="Found neighbour nr of index "+QString::number(nn)+" in Nh. ";
                            step = NR_IN_NH;
                            return true;
                        } else if(std::find(Nw.begin(), Nw.end(), H.getSingleAdjecencyList(n)[i]) != Nw.end()){
                            nn = H.getSingleAdjecencyList(n)[i];
                            highlighted.push_back(nn);
                            stepInfo+="Found neighbour nr of index "+QString::number(nn)+" in Nw. ";
                            step = NR_IN_NW;
                            return true;
                        }
                    }
                    stepInfo+="Haven't found right neighbour. Checking if C is a clique.";
                    step = CHECK_C_CLI;
                } return true;
           case NR_IN_NH:
                if(vlh[findIdxInSet(Nh, nn)] == G.getNodesIndexes()[G.getNodeCount()-1]){
                    stepInfo = "nr is connected to the same verticle in G n"
                            +QString::number(n)+" was. Looking for another node in n's neighbourhood.";
                } else {
                    stepInfo = "nr is not connected to the same verticle in G n"
                            +QString::number(n)+". Add nr to C, remove it form Nh, and connect its"
                         " node to the one nwely created. Looking for next neighbour.";
                    C.push_back(nn);
                    connectFromNhToSecondNode(nn, G.getNodesIndexes()[G.getNodeCount()-1]);
                }
                step = EACH_NR_IN_NB;
                return true;
           case NR_IN_NW:
                stepInfo = "Add nr to C, Nh, set "+QString::number(nn)+"'s one connected node "
                           "to the newly created one. Remove nr form Nw. Looking for next neighbour.";
                C.push_back(nn);
                addToNh(G.getNodesIndexes()[G.getNodeCount()-1], nn);
                step = EACH_NR_IN_NB;
                return true;
           case CHECK_C_CLI:
                if(C.size() !=0){
                    if(isAClique(C)){
                        stepInfo="C is not empty and but is a clique. Checking if Nh is empty.";
                    }else{
                        stepInfo="C is not empty and not a clique. H is not a line graph.";
                        step = DONE;
                        return true;
                    }
                }else{
                    stepInfo="C is empty, checking if Nh is empty.";
                }
                step = IS_NH_EMPTY;
                return true;
           case IS_NH_EMPTY:
                if(Nh.size()==0){
                    stepInfo="Nh is empty, finished graph reconstruction";
                    step = DONE;
                } else {
                    stepInfo="Nh is not empty.";
                    step = WHILE_NH;
                }

           case DONE:
                stepInfo = "DONE.";
                return true;
            }
    return true;
}

int Iligra::findIdxInSet(std::vector<int> & set, int n){
    for(int i =0 ; i<set.size();++i){
        if(set[i]==n){
            return i;
        }
    }
    return -1;
}

void Iligra::connectFromNhToSecondNode(int Nhidx, int idxNewNode){

    std::vector<int>::iterator iNh = Nh.begin()+Nhidx;
    std::vector<int>::iterator iVlh = vlh.begin()+Nhidx;
    G.connect(idxNewNode, *iVlh);
    G.connectHangingLine(*iVlh,idxNewNode,  Nh[Nhidx]);
    Nh.erase(iNh);
    vlh.erase(iVlh);
    return;
}

std::vector<int> Iligra::getNeighboursN1WithoutJ(){
    std::vector<int> nbn = H.getSingleAdjecencyList(H.getNodesIndexes()[0]);
    std::vector<int> out;
    for(std::vector<int>::iterator it=nbn.begin(); it<nbn.end();++it){
        if(std::find(J.begin(), J.end(), *it) == J.end()){
            out.push_back(*it);
        }
    }
    return out;
}


void Iligra::connectAllJToV1(){
    for(std::vector<int>::iterator it=J.begin();it<J.end();++it){
        addToNh(G.getNodesIndexes()[0],*it);
    }
}

bool Iligra::isAClique(std::vector<int> potentialClique){
    int startingPoint = 1;
    for(int nodeIdx = 0; nodeIdx < potentialClique.size()-1; ++nodeIdx){
        for(int i = startingPoint; i<potentialClique.size();++i){
            if(!checkIfIsNeighbour(potentialClique[nodeIdx], potentialClique[i])){
                return false;
            }
        }
        ++startingPoint;
    }
    return true;
}


void Iligra::addToNh(int v, int n){
    G.addAdjecentToANode(-10, v);
    G.addLine(v, -10,  n);
    Nh.push_back(n);
    vlh.push_back(v);
    for(std::vector<int>::iterator it = Nw.begin(); it<Nw.end(); ++it){
        if(*it == n){
            Nw.erase(it);
            break;
        }
    }
    return;
}

bool Iligra::isnsInC(){
    int ns = getns();
    if(checkIfIsNeighbour(0, ns) || checkIfIsNeighbour(1, ns) ){
        return true;
    }
    return false;
}

bool Iligra::isnxInC(){
    int nx = getnx();
    if(checkIfIsNeighbour(0, nx) || checkIfIsNeighbour(1, nx) ){
        return true;
    }
    return false;
}

void Iligra::addNuConnectionToG(int i){
    //G.addAdjecentToANode(-10, G.getNodesIndexes()[i]);
    G.addLine(G.getNodesIndexes()[i], -10,  nu);
    Nh.push_back(nu);
    vlh.push_back(G.getNodesIndexes()[i]);
    for(std::vector<int>::iterator it = Nw.begin(); it<Nw.end(); ++it){
        if(*it == nu){
            Nw.erase(it);
            break;
        }
    }
    for(std::vector<int>::iterator it = J.begin(); it<J.end(); ++it){
        if(*it == nu){
            J.erase(it);
            break;
        }
    }
    return;
}

bool Iligra::checkIfIsNeighbour(int idx, int idxPot){
    std::vector<int> nbn = H.getSingleAdjecencyList(idx);
    return std::find(nbn.begin(), nbn.end(), idxPot) != nbn.end();
}

int Iligra::getnt(){
    std::vector<int> nbn = H.getSingleAdjecencyList(nu);
    std::vector<int> nb1 = H.getSingleAdjecencyList(H.getNodesIndexes()[0]);
    std::vector<int> nb2 = H.getSingleAdjecencyList(H.getNodesIndexes()[1]);
    int foundBefore = 0;
    for( std::vector<int>::iterator in = nbn.begin(); in < nbn.end(); ++in){
        if(!(std::find(nb1.begin(), nb1.end(), *in) != nb1.end() ||
            std::find(nb2.begin(), nb2.end(), *in) != nb2.end())){
            if(!foundBefore){
                ++foundBefore;
            }else{
                return *in;
            }
        }
    }
    return -1;
}

int Iligra::getns(){
    std::vector<int> nbn = H.getSingleAdjecencyList(nu);
    std::vector<int> nb1 = H.getSingleAdjecencyList(H.getNodesIndexes()[0]);
    std::vector<int> nb2 = H.getSingleAdjecencyList(H.getNodesIndexes()[1]);
    for( std::vector<int>::iterator in = nbn.begin(); in < nbn.end(); ++in){
        if(!(std::find(nb1.begin(), nb1.end(), *in) != nb1.end() ||
            std::find(nb2.begin(), nb2.end(), *in) != nb2.end())){
                return *in;
        }
    }
    return -1;
}

int Iligra::getnr(){
    if(nu>-1){
        for(int i=0; i<2;++i){
            if(J[i] != nu){
                return J[i];
            }
        }
    }
    return -1;
}

int Iligra::getnx(){
    int ns = getns();
    for(std::vector<int>::iterator it = Nw.begin(); it<Nw.end(); ++it){
        if(*it != ns && *it != nu){
            return *it;
        }
    }
    return -1;
}

int Iligra::getZSize(int idx){
    std::vector<int> nbn = H.getSingleAdjecencyList(idx);
    std::vector<int> nb1 = H.getSingleAdjecencyList(H.getNodesIndexes()[0]);
    std::vector<int> nb2 = H.getSingleAdjecencyList(H.getNodesIndexes()[1]);
    int zSize = 0;
    std::vector<int>::iterator in = nbn.begin();
    for(; in < nbn.end(); ++in){
        if(!(std::find(nb1.begin(), nb1.end(), *in) != nb1.end() ||
            std::find(nb2.begin(), nb2.end(), *in) != nb2.end())){
            ++zSize;
        }
    }
    return zSize;
}

int Iligra::getNodeWithLessThanThreeZ(){
    //Nw  = H.getNodesIndexes \ n1 n2 (n1\n2)
    //Nh Nb[0] (n1\n2)
    //Nb Nw[0].adjecencyList()
    //vlh v1 (v2)
    //highlighted = n1 n2 (n1\n2)
    //G -v1-v2
    // J (n1 n n2)
    for(std::vector<int>::iterator it = J.begin(); it<J.end(); ++it){
        int Z = getZSize(*it);
        if(Z<=2){
            return *it;
        }
    }
    return -1;
}


void Iligra::setNu(int nu){
    //Nw  = H.getNodesIndexes \ n1 n2 (n1\n2) nu
    //Nh Nb[0] (n1\n2) nu
    //Nb Nw[0].adjecencyList()
    //vlh v1 (v2) v2
    //highlighted = n1 n2 (n1\n2)
    //G -v1-v2
    // J (n1 n n2)\nu
    Nh.push_back(nu);
    vlh.push_back(G.getNodesIndexes()[1]);
    std::vector<int>::iterator i = std::find(J.begin(), J.end(), nu);
    J.erase(i);
    i = std::find(Nw.begin(), Nw.end(), nu);
    Nw.erase(i);
}

int Iligra::rightNExistInJ_three(){
    for(std::vector<int>::iterator it = J.begin(); it<J.end(); ++it){
        std::vector<int> nbn = H.getSingleAdjecencyList(*it);
        std::vector<int>::iterator in = nbn.begin();
        for(; in < nbn.end(); ++in){
            if(std::find(J.begin(), J.end(), *in) != J.end()){
                break;
            }
        }
        if(in == nbn.end()){
            return *it;
        }
    }
    return -1;
}

int Iligra::rightNExistInJ_oneTwo(){
    for(std::vector<int>::iterator it = J.begin(); it<J.end(); ++it){
        std::vector<int> nbn = H.getSingleAdjecencyList(*it);
        std::vector<int> nb1 = H.getSingleAdjecencyList(H.getNodesIndexes()[0]);
        std::vector<int> nb2 = H.getSingleAdjecencyList(H.getNodesIndexes()[1]);
        if(nbn.size()<3){
            break;
        } else {
            std::vector<int>::iterator in = nbn.begin();
            for(; in < nbn.end(); ++in){
                if(!(std::find(nb1.begin(), nb1.end(), *in) != nb1.end() ||
                   std::find(nb2.begin(), nb2.end(), *in) != nb2.end())){
                    break;
                }
            }
            if(in == nbn.end()){
                return *it;
            }
        }
    }
    return -1;
}


void Iligra::n1OnlyNeighbours(){
    //Nw  = H.getNodesIndexes \ n1 n2 (n1\n2)
    //Nh Nb[0] (n1\n2)
    //Nb Nw[0].adjecencyList()
    //vlh v1 (v2)
    //highlighted = n1 n2 (n1\n2)
    //G -v1-v2
    // J (n1 n n2)
    std::vector<int> nb2 = H.getSingleAdjecencyList(Nb[0]);
    for(std::vector<int>::iterator it = ++Nb.begin(); it<Nb.end(); ++it){
        std::vector<int>::iterator it2 = nb2.begin();
        for(; it2<nb2.end(); ++it2){
            if(*it == *it2){
                J.push_back(*it);
                break;
            }
        }
        if(it2 == nb2.end()){
            Nh.push_back(*it);
            highlighted.push_back(*it);
            for(std::vector<int>::iterator itw = Nw.begin(); itw<Nw.end(); ++itw){
                if(*itw == *it){
                    Nw.erase(itw);
                    break;
                }
            }
            vlh.push_back(G.getNodesIndexes()[1]);
        }
    }
}

void Iligra::firstConnectN2(){
    //Nw  = H.getNodesIndexes \ n1 n2
    //Nh Nb[0]
    //Nb Nw[0].adjecencyList()
    //vlh v1
    //highlighted = Nw[0]
    //G -v1-v2
    //G.addAdjecentToANode(-10, G.getNodesIndexes()[0]);
    G.addLine(G.getNodesIndexes()[0], -10, 1);
    //G.getLine(G.getNodesIndexes()[0], -10)->setLabel(1, -1);
    Nh.push_back(H.getNodesIndexes()[1]);
    vlh.push_back(G.getNodesIndexes()[0]);
    for(std::vector<int>::iterator it = Nw.begin(); it<Nw.end(); ++it){
        if(*it == Nh[0]){
            Nw.erase(it);
            break;
        }
    }
    return;
}

void Iligra::connectN1(){
    //Nw  = H.getNodesIndexes \ Nw[0]
    //Nh empty
    //Nb Nw[0].adjecencyList()
    //vlh empty
    //highlighted = Nw[0]
    //G v1-v2
    G.connect(0,1);
    G.getLine(0, 1)->setLabel(0, -1);
    Nw.erase(Nw.begin());
    return;
}

void Iligra::addFirstNodes(){
    //Nw  = H.getNodesIndexes
    //Nh empty
    //Nb Nw[0].adjecencyList()
    //vlh empty
    //highlighted = Nw[0]
    //G v1 v2
    G.addNode();
    G.addNode();
    return;
}

int Iligra::choseArbitraryNeighbour(){
    //Nw  = H.getNodesIndexes
    //Nh empty
    //Nb Nw[0].adjecencyList()
    //vlh empty
    //highlighted = Nw[0] Nb[0]
    //G empty
    highlighted.push_back(Nb[0]);
    return Nb[0];
}

int Iligra::choseArbitraryNode(){
    //Nw  = H.getNodesIndexes
    //Nh empty
    //Nb Nw[0].adjecencyList()
    //vlh empty
    //highlighted = Nw[0]
    //G epmty
    highlighted.push_back(Nw[0]);
    Nb = H.getSingleAdjecencyList(Nw[0]);
    return Nw[0];
}


bool Iligra::changeStep(Step newStep){
    step = newStep;
    return true;
}

bool Iligra::loadFromFile(QString file){
    QFile graphFile(file);
    if (!graphFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    QTextStream in(&graphFile);
    //std::vector<int> indexes;
    std::vector<std::vector<int>> adjecencyList, tmp;
    while (!in.atEnd()) {
        QString line = in.readLine();
        std::vector<int> inner;
        QStringList l = line.split(":");
        //inner.push_back(l[0].toInt());
        l = l[1].split(" ");
        for(auto &li : l){
            inner.push_back(li.toInt());
        }
        adjecencyList.push_back(inner);
    }
    /*for(std::vector<std::vector<int>>::iterator it1 = adjecencyList.begin();
        it1 < adjecencyList.end(); ++it1){


        std::cout<<"##############"<<std::endl;
        std::cout<<adjecencyList.size()<<std::endl;

        std::cout<<"##############"<<std::endl;

        if((*it1)[0]>=MAX_NODES){
            adjecencyList.erase(it1--);
            continue;
        }
        for(std::vector<int>::iterator i = ++(*it1).begin();
            i<(*it1).end();++i){
            if(*i >= MAX_NODES){
               (*it1).erase(i--);
            }
        }
        if(it1 == adjecencyList.begin()){
            continue;
        }
        std::vector<std::vector<int>>::iterator it2 = it1;

        while(true){
            if((*it1)[0] == (*--it2)[0]){
                adjecencyList.erase(it1--);
                break;
            } else if(!(*it1)[0]>(*(it2))[0]){
                break;
            }

        }
        if((*it1)[0] < (*it2)[0]){
            std::vector<int> temp = (*it1);
            adjecencyList.insert(it2, temp);
            adjecencyList.erase(it1--);
        }

    }*/
    std::cout<<"##############"<<std::endl;
    H.clear();
    for(std::vector<std::vector<int>>::iterator it1 = adjecencyList.begin();
        it1 < adjecencyList.end(); ++it1){
        int idx = (*it1)[0];
        (*it1).erase((*it1).begin());
        H.addNode(idx, (*it1));
    }
    Nw = H.getNodesIndexes();
    std::cout<<"##############"<<std::endl;
    std::cout<<H.getNodesIndexes()[2]<<std::endl;
    H.printAdjecencyList();
    changeStep(LOADED);
    return true;
}
