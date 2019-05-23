#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <bitset>
#include <array>
#include <memory>

#define MAX_NODES 20

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
    private:
        int index_;
        std::vector<int> adjecencyList_;
    };

    Graph();
    int addNode();
    int addNode(const std::vector<int> &adjList);
    int addNode(int idx, const std::vector<int> &adjList);
    bool removeNode(int idx);
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

private:

    std::vector<std::unique_ptr<Node>> nodes_;
    std::bitset<MAX_NODES> indexList_;

};

#endif // GRAPH_H
