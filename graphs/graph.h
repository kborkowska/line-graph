#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <bitset>

#define MAX_NODES 50

class Graph
{
public:
    Graph();
    int addNode();
    int addNode(const std::vector<int> &adjList);
    bool removeNode(int idx);
    void printAdjecencyList();
    int** getAdjecencyList();

private:
    class Node
    {
    public:
        Node(int index);
        Node(int index, const std::vector<int> &adjecencyList);
        bool addAdjecent(int idx);
        bool setAdjecencyList(const std::vector<int> &adjecencyList);
        bool removeAdjecent(int idx);
        std::vector<int> getAdjecenyList();
        int getIndex();
        bool setIndex(int idx);
    private:
        std::vector<int> adjecencyList_;
        int index_;
    };

    std::vector<*Node> nodes_;
    std::bitset<MAX_NODES> indexList_;

};

#endif // GRAPH_H
