#include "graph.h"

#include <iostream>
#include <cstddef>

Graph::Graph(){
    colorList << "olivedrab"
              << "orange"
              << "orangered"
              << "orchid"
              << "palegreen"
              << "paleturquoise"
              << "palevioletred"
              << "papayawhip"
              << "peachpuff"
              << "peru"
              << "pink"
              << "plum"
              << "powderblue"
              << "purple"
              << "rosybrown"
              << "royalblue"
              << "saddlebrown"
              << "salmon"
              << "sandybrown"
              << "seagreen";
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
    getNode(idx)->alphaIndex = 'A' + idx;
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
            indexList_[idx] = false;
            result = true;
            break;
        }
    }
    return result;
}

void Graph::clear(){
    std::vector<int> idxs = getNodesIndexes();
    for(auto &idx: idxs){
        removeNode(idx);
    }
    return;
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
    if(!containsLine(adjIdx, nodeIdx)) {
        lines_.push_back(std::make_unique<Graph::Line>(adjIdx, nodeIdx, -1));
    }
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

void Graph::Node::setColor(QString color) {
    color_ = color;
}

QString Graph::Node::getColor() {
    return color_;
}

void Graph::Node::setHighlighted(bool highlighted) {
    highlighted_ = highlighted;
}

bool Graph::Node::isHighlighted(){
    return highlighted_;
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
        int x = rand()%MAX_X;
        int y = rand()%MAX_Y;
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
                if ((nodePosition == destPosition) && nodeIterator != destIterator) {
                    std::cout << nodeIterator << " " << destIterator<< std::endl;
                    QRandomGenerator gen(nodeIterator + 1);
                    int x = rand()%MAX_X;
                    int y = rand()%MAX_Y;
                    setNodePosition(nodeIterator, QPoint(x, y));
                    nodePosition = getNodePosition(nodeIterator);
                }
                if (nodeIterator == destIterator) {
                    continue;
                }
                //std::cout << "Node: " << nodeIterator << ", Destination: " << destIterator << std::endl;
                //std::cout << "(" << nodePosition.x() << "," << nodePosition.y() << ")" << " "
                //          << "(" << destPosition.x() << "," << destPosition.y() << ")" << std::endl;
                QLineF vec(nodePosition, destPosition);
                qreal d = vec.length();
                //std::cout <<  vec.dx() << " " << vec.dy() << " " << d << std::endl;
                if(std::find(adjacent.begin(), adjacent.end(), destIterator) != adjacent.end()) {
                    qreal fa = (d*d) / K_CONSTANT;
                    xForce += fa * vec.dx() / d;
                    yForce += fa * vec.dy() / d;
                    //std::cout << "fa " << fa << std::endl;
                }

                qreal fr = (C_CONSTANT*K_CONSTANT*K_CONSTANT)/d;
                xForce -= fr * vec.dx() / d;
                yForce -= fr * vec.dy() / d;
                //std::cout << "fr " << fr << std::endl;
            }
            //std::cout << (int)xForce << " " << (int)yForce << std::endl;
            nodePosition.setX(qMin(qMax(nodePosition.x() + (int)(xForce), RADIUS), MAX_X - RADIUS));
            nodePosition.setY(qMin(qMax(nodePosition.y() + (int)(yForce), RADIUS), MAX_Y - RADIUS));

            setNodePosition(nodeIterator, nodePosition);
        }
    }
}

void Graph::colorNodes() {
    for (int nodeIterator = 0; nodeIterator < getNodeCount(); nodeIterator++) {
        getNode(nodeIterator)->setColor(colorList.at(nodeIterator));
    }
}

Graph::Line::Line(int node1, int node2, int label) {
    this->node1 = node1;
    this->node2 = node2;
    this->label1 = label;
}

void Graph::fillLines() {
    for (int nodeIterator = 0; nodeIterator < getNodeCount(); nodeIterator++) {
        std::vector<int> adjacentList = getSingleAdjecencyList(nodeIterator);
        for (int adjacent: adjacentList) {
            if (!containsLine(nodeIterator, adjacent)) {
                lines_.push_back(std::make_unique<Graph::Line>(nodeIterator, adjacent, -1));
            }
        }
    }
}

bool Graph::containsLine (int node1, int node2) {
    for(auto& line: lines_){
        if((line->node1 == node1 && line->node2 == node2) || (line->node1 == node2 && line->node2 == node1)) {
            return true;
        }
    }
    return false;
}

Graph::Line* Graph::getLine (int node1, int node2, int label) {
    for(std::vector<std::unique_ptr<Graph::Line>>::iterator iter=lines_.begin(); iter<lines_.end(); ++iter){
        if((((*iter)->node1 == node1 && (*iter)->node2 == node2) || ((*iter)->node1 == node2 && (*iter)->node2 == node1))){
            if (label == (*iter)->label1 || label == -1) {
                return iter->get();
            }
        }
    }
    return nullptr;
}

QString Graph::Line::getLabel() {
    return label_;
}

void Graph::Line::setLabel(QString label) {
    label_ = label;
}

void Graph::addLine(int node1, int node2, int label) {
    if (!containsLine(node1, node2) || node1 == -10 || node2 == -10) {
        lines_.push_back(std::make_unique<Graph::Line>(node1, node2, label));
    }
}

void Graph::connectHangingLine(int node1, int node2, int label) {
    Line *line = getLine(node1, node2, label);
    if ((node1 >=0 && node2 == -10)){
        if (line != nullptr) {
            line->node2 = node2;
            return;
        }
    }
    if ((node2 >=0 && node1 == -10)){
        if (line != nullptr) {
            line->node1 = node1;
            return;
        }
    }
}
