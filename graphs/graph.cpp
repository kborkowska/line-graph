#include "graph.h"

#include <iostream>
#include <cstddef>

Graph::Graph(){
}

int Graph::addNode(){
    int idx;
    for(idx = 0; idx < MAX_NODES; ++idx){
        if(indexList_[idx] == false){
            nodes_.push_back(std::make_unique<Node>(idx));
            indexList_[idx]=true;
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

Graph::Node* Graph::getNode(int idx){
    for(std::vector<std::unique_ptr<Node>>::iterator iter=nodes_.begin(); iter<nodes_.end(); ++iter){
        if((*iter)->getIndex() == idx){
            return iter->get();
        }
    }
    return nullptr;
}

int Graph::addNode(int idx, const std::vector<int> &adjecencyList){
    if(!indexList_[idx]){
        nodes_.push_back(std::make_unique<Node>(idx,adjecencyList));
        indexList_[idx]=true;
    } else {
        return addNode(adjecencyList);
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

std::vector<int> Graph::getNodesIndexes(){
    std::vector<int> indexes;
    for(auto& node: nodes_){
        indexes.push_back(node->getIndex());
    }
    return indexes;
}

int Graph::getNodeCount(){
    return nodes_.size();
}

bool Graph::changeIndex(int oldIdx, int newIdx){
    bool result = false;
    if(!indexList_[newIdx]){
        for(auto& node: nodes_){
            if(node->getIndex()==oldIdx){
                node->setIndex(newIdx);
                indexList_[oldIdx] = false;
                indexList_[newIdx] = true;
                result = true;
                break;
            }
        }
    }
    return result;
}

bool Graph::setSingleAdjecencyList(int nodeIdx, const std::vector<int> &adjecencyList){
    bool result = false;
    for(auto& node: nodes_){
        if(node->getIndex()==nodeIdx){
            node->setAdjecencyList(adjecencyList);
            result = true;
            break;
        }
    }
    return result;
}

std::vector<int> Graph::getSingleAdjecencyList(int nodeIdx){
    std::vector<int> al;
    for(auto& node: nodes_){
        if(node->getIndex()==nodeIdx){
            al = node->getAdjecencyList();
        }
    }
    return al;
}

bool Graph::addAdjecentToANode(int adjIdx, int nodeIdx){
    bool result = false;
    for(auto& node: nodes_){
        if(node->getIndex()==nodeIdx){
            if(node->addAdjecent(adjIdx))
                result = true;
            break;
        }
    }
    return result;
}

bool Graph::removeAdjecentFromANode(int adjIdx, int nodeIdx){
    bool result = false;
    for(auto& node: nodes_){
        if(node->getIndex()==nodeIdx){
            if(node->removeAdjecent(adjIdx))
                result = true;
            break;
        }
    }
    return result;
}

bool Graph::connect(int adjIdx, int nodeIdx){
    bool res1 = addAdjecentToANode(adjIdx, nodeIdx);
    bool res2 = addAdjecentToANode(nodeIdx, adjIdx);
    return res1 && res2;
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

void Graph::Node::setPosition(QPoint position) {
    position_ = position;
}

QPoint Graph::Node::getPosition() {
    return position_;
}

void Graph::setNodePosition(int idx, QPoint position) {
    for(auto& node: nodes_){
        if(node->getIndex() == idx){
            node->setPosition(position);
        }
    }
}

QPoint Graph::getNodePosition(int idx) {
    for(auto& node: nodes_){
        if(node->getIndex() == idx){
            return node->getPosition();
        }
    }
}

void Graph::repositionNodes() {
    for(int nodeIterator = 0; nodeIterator < getNodeCount(); nodeIterator++) {
        QRandomGenerator gen(nodeIterator + 1);
        int x = gen.bounded(0, MAX_X);
        int y = gen.bounded(0, MAX_Y);
        setNodePosition(nodeIterator, QPoint(x, y));
    }

    for (int i = 0; i < 100; i++) {
        for (int nodeIterator = 0; nodeIterator < getNodeCount(); nodeIterator++) {
            qreal xForce = 0;
            qreal yForce = 0;
            QPoint nodePosition = getNodePosition(nodeIterator);

            std::vector<int> adjacent = getSingleAdjecencyList(nodeIterator);
            for (int destIterator = 0; destIterator < getNodeCount(); destIterator++) {
                QPoint destPosition = getNodePosition(destIterator);
                if (nodeIterator == destIterator) {
                    continue;
                }
                std::cout << "Node: " << nodeIterator << ", Destination: " << destIterator << std::endl;
                std::cout << "(" << nodePosition.x() << "," << nodePosition.y() << ")" << " "
                          << "(" << destPosition.x() << "," << destPosition.y() << ")" << std::endl;
                QLineF vec(nodePosition, destPosition);
                qreal d = vec.length();
                std::cout <<  vec.dx() << " " << vec.dy() << " " << d << std::endl;
                if(std::find(adjacent.begin(), adjacent.end(), destIterator) != adjacent.end()) {
                    qreal fa = (d*d) / K_CONSTANT;
                    xForce += fa * vec.dx() / d;
                    yForce += fa * vec.dy() / d;
                    std::cout << "fa " << fa << std::endl;
                }

                qreal fr = (C_CONSTANT*K_CONSTANT*K_CONSTANT)/d;
                xForce -= fr * vec.dx() / d;
                yForce -= fr * vec.dy() / d;
                std::cout << "fr " << fr << std::endl;
            }
            std::cout << (int)xForce << " " << (int)yForce << std::endl;
            nodePosition.setX(qMin(qMax(nodePosition.x() + (int)(xForce), RADIUS), MAX_X - RADIUS));
            nodePosition.setY(qMin(qMax(nodePosition.y() + (int)(yForce), RADIUS), MAX_Y - RADIUS));
            setNodePosition(nodeIterator, nodePosition);
        }
    }
}
