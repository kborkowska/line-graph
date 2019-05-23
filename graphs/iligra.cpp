#include "iligra.h"

#include <QFile>
#include <QStringList>
#include <QTextStream>

Iligra::Iligra(){
    step = NONE;
    stepInfo = "Load a line graph";
}


bool Iligra::changeState(){

    switch(step){
          case NONE:          stepInfo = "Can't move to the next step!\n Load a line graph";
                              return false;
          case LOADED:        step = ARBITRARY_N1;
                              stepInfo = "Chose one arbitrary node";
                              choseArbitraryNode();
                              return true;
          case ARBITRARY_N1:  stepInfo = "Chose one of its neighboours";
                              step = ARBITRARY_N2;
                              choseArbitraryNeighbour();


    }
    return true;
}

bool Iligra::changeStep(Step newStep){
    step = newStep;
    return true;
}

int Iligra::choseArbitraryNode(){
    highlighted.push_back(Nw[0]);
    Nb = H.getSingleAdjecencyList(Nw[0]);
    return Nw[0];
}

int Iligra::choseArbitraryNeighbour(){
    highlighted.push_back(Nb[0]);
    return Nb[0];
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
    for(std::vector<std::vector<int>>::iterator it1 = adjecencyList.begin();
        it1 < adjecencyList.end(); ++it1){
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
        std::vector<std::vector<int>>::iterator it2 = --it1;
        while((*it1)[0]<(*it2)[0] && it2>adjecencyList.begin()){
            --it2;
        }
        if((*it1)[0] == (*it2)[0]){
            adjecencyList.erase(it1--);
        } else if(++it2!=it1){
            adjecencyList.insert(it2, *it1);
            adjecencyList.erase(it1);
        }
    }

    for(std::vector<std::vector<int>>::iterator it1 = adjecencyList.begin();
        it1 < adjecencyList.end(); ++it1){
        int idx = (*it1)[0];
        (*it1).erase((*it1).begin());
        H.addNode(idx, (*it1));
    }

    Nw = H.getNodesIndexes();

    changeStep(LOADED);

    return true;
}