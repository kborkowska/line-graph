#include "iligra.h"

#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <iostream>

Iligra::Iligra(){
    step = NONE;
    stepInfo = "Load a line graph";
    std::cout<<stepInfo.toStdString()<<std::endl;
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
          case ANALYSE_J:       {if(J.size()==1 || J.size()==2){
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
          case ONE_TWO_J:       {int nu = rightNExistInJ_oneTwo();
                                 if(nu>0){
                                    stepInfo = "Found right nu. Remove it form J and Nw, add to Nh."
                                               "Set nu as verticle of v2";
                                    step = NU;
                                    setNu(nu);
                                 } else {
                                     stepInfo = "Haven't found right nu, commencing special cases scenario";
                                     step = INIT_SPECIAL;

                                 }
                                 return true;
                                }
           case THREE_J:        {int nu = rightNExistInJ_three();
                                 if(nu>0){
                                     stepInfo = "Found right nu. Remove it form J and Nw, add to Nh."
                                                "Set nu as verticle of v2";
                                     setNu(nu);
                                 } else {
                                     stepInfo = "Haven't found right nu. Moving on.";
                                 }
                                 step = NU;
                                }
           case INIT_SPECIAL:   {int nu = getNodeWithLessThanThreeZ();
                                 if(nu>0){
                                     stepInfo = "Found nu for special cases: " + QString::number(nu);
                                     step = SPECIAL;
                                 } else {
                                     stepInfo = "No proper candidate for special cases scenario";
                                     step = EACH_IN_J;
                                 }

                                }
    }
    return true;
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
        std::vector<int> nbn = H.getSingleAdjecencyList(*it);
        std::vector<int> nb1 = H.getSingleAdjecencyList(highlighted[0]);
        std::vector<int> nb2 = H.getSingleAdjecencyList(highlighted[1]);
        int Zsize = 0;
        std::vector<int>::iterator in = nbn.begin();
        for(; in < nbn.end(); ++in){
            if(!(std::find(nb1.begin(), nb1.end(), *in) != nb1.end() ||
                std::find(nb2.begin(), nb2.end(), *in) != nb2.end())){
                ++Zsize;
            }
            if(Zsize<=2){
                return *it;
            }
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
        std::vector<int> nb1 = H.getSingleAdjecencyList(highlighted[0]);
        std::vector<int> nb2 = H.getSingleAdjecencyList(highlighted[1]);
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
                    Nw.erase(it);
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
    G.addAdjecentToANode(-10, G.getNodesIndexes()[0]);
    Nh.push_back(highlighted[1]);
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
        inner.push_back(l[0].toInt());
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
    changeStep(LOADED);
    return true;
}
