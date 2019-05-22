#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <bitset>
#include <array>
#include <memory>

#define MAX_NODES 5

class Graph
{
public:
    Graph();
    int addNode();
    int addNode(const std::vector<int> &adjList);
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

private:
    class Node
    {
    public:
        Node(int index);
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

    std::vector<std::unique_ptr<Node>> nodes_;
    std::bitset<MAX_NODES> indexList_;

};

#endif // GRAPH_H
