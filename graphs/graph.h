#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <bitset>
#include <array>
#include <memory>
#include <QPoint>
#include <QLineF>
#include <QRandomGenerator>
#include <QString>
#include <QStringList>
#include "math.h"

#define MAX_NODES 20
#define K_CONSTANT 550
#define C_CONSTANT 0.009
#define MAX_X 400
#define MAX_Y 300
#define RADIUS 15

class Graph
{
public:
    class Node
    {
    public:
        Node(int index = -1);
        Node(int index, const std::vector<int> &adjecencyList);
        bool addAdjecent(int idx);
        bool setAdjecencyList(const std::vector<int> &adjecencyList);
        bool removeAdjecent(int idx);
        std::vector<int> getAdjecencyList();
        int getIndex();
        int setIndex(int idx);
        void setPosition(QPoint position);
        QPoint getPosition();
        void setColor(QString color);
        QString getColor();
        bool isHighlighted();
        void setHighlighted(bool highlighted);


        char alphaIndex;
    private:
        int index_;
        std::vector<int> adjecencyList_;
        QPoint position_;
        QString color_ = "white";
        bool highlighted_ = false;
    };

    class Line {
    public:
        Line();
        Line(int node1, int node2);
        Line(int node1, int node2, int label);
        int node1 = -1, node2 = -1;
        int label1 = -1, label2 = -1;
        QString label_ = "";
        QString getLabel();
        void setLabel(QString label);
        void setLabel(int label1, int label2) { this->label1 =  label1; this->label2 = label2;}
    };

    Graph();
    int addNode();
    int addNode(const std::vector<int> &adjList);
    int addNode(int idx, const std::vector<int> &adjList);
    bool removeNode(int idx);
    void clear();
    void printAdjecencyList();
    std::vector<std::vector<int>> getAdjecencyList();
    std::vector<int> getNodesIndexes();
    int getNodeCount();
    bool changeIndex(int oldIdx, int newIdx);
    bool setSingleAdjecencyList(int nodeIdx, const std::vector<int> &adjecencyList);
    std::vector<int> getSingleAdjecencyList(int nodeIdx);
    bool addAdjecentToANode(int adjIdx, int nodeIdx);
    bool removeAdjecentFromANode(int adjIdx, int nodeIdx);
    Node* getNode(int idx);
    bool connect(int adjIdx, int nodeIdx);

    void setNodePosition(int idx, QPoint position);
    QPoint getNodePosition(int idx);
    void repositionNodes();
    void colorNodes();
    void fillLines();
    bool containsLine(int node1, int node2);
    Line* getLine(int node1, int node2, int label = -1);
    int getLineCount() {return lines_.size();}
    void addLine(int node1, int node2, int label = -1);
    void connectHangingLine(int node1, int node2, int label);
    std::vector<std::unique_ptr<Line>> lines_;
    bool alphabeticalIndexing = false;
    QStringList colorList;
private:

    std::vector<std::unique_ptr<Node>> nodes_;

    std::bitset<MAX_NODES> indexList_;


};

#endif // GRAPH_H
