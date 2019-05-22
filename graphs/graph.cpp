#include "graph.h"

#include <iostream>

Graph::Graph(){}

int Graph::addNode(){
    int idx;
    for(idx = 0; idx < MAX_NODES; ++idx){
        if(indexList_[idx] == false){
            nodes_.push_back(std::make_unique<Node>(idx));
            break;
        }
    }
    return idx;
}

int Graph::addNode(const std::vector<int> &adjecencyList){
    int idx;
    for(idx = 0; idx < MAX_NODES; ++idx){
        if(indexList_[idx] == false){
            nodes_.push_back(std::make_unique<Node>(idx,adjecencyList));
            indexList_[idx]=true;
            break;
        }
    }
    return idx;
}

bool Graph::removeNode(int idx){
    bool result = false;
    for(std::vector<std::unique_ptr<Node>>::iterator iter=nodes_.begin(); iter<nodes_.end(); ++iter){
        if((*iter)->getIndex() == idx){
            nodes_.erase(iter);
            result = true;
            break;
        }
    }
    return result;
}

std::vector<std::vector<int>> Graph::getAdjecencyList(){
    std::vector<std::vector<int>> adjecencyList;
    for(auto &node: nodes_){
        adjecencyList.push_back(node->getAdjecencyList());
    }
    return adjecencyList;
}

void Graph::printAdjecencyList(){
    std::cout<<"PRINTING ADJECENCY LIST"<<std::endl;
    for(auto &node: nodes_){
        std::cout<<"Node #"<<node->getIndex()<<": ";
        std::vector<int> adl = node->getAdjecencyList();
        for(auto &idx: adl){
            std::cout<<idx<<"; ";
        }
        std::cout<<std::endl;
    }
    return;
}

// Node methods
Graph::Node::Node(int index):index_(index){}

Graph::Node::Node(int index, const std::vector<int> &adjecencyList):
                    index_(index), adjecencyList_(adjecencyList){}

bool Graph::Node::addAdjecent(int idx){
    adjecencyList_.push_back(idx);
    return true;
}

bool Graph::Node::setAdjecencyList(const std::vector<int> &adjecencyList){
    adjecencyList_ = adjecencyList;
    return true;
}

bool Graph::Node::removeAdjecent(int idx){
    bool result = false;
    for(std::vector<int>::iterator iter = adjecencyList_.begin(); iter<adjecencyList_.end();++iter){
        if(*iter == idx){
            adjecencyList_.erase(iter);
            result = true;
            break;
        }
    }
    return result;
}

std::vector<int> Graph::Node::getAdjecencyList(){
    return adjecencyList_;
}

int Graph::Node::getIndex(){
    return index_;
}

int Graph::Node::setIndex(int idx){
    index_ = idx;
    return idx;
}
