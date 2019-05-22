#include <QtTest>
#include <vector>

#include "../graph.h"

class ClassTest : public QObject
{
    Q_OBJECT

public:
    ClassTest();
    ~ClassTest();

private slots:
    //void initTestCase();
    //void cleanupTestCase();
    void test_addOneNode();
    void test_addMoreThanOneNode();
    void test_addNodeWithAdjList();
    void test_removeNode();
    void test_getNodesIndexes();
    void test_changeIndex();
    void test_setSingleAdjecencyList();
    void test_getSingleAdjcencyList();
    void test_addAdjecentToANode();
    void test_removeAdjecentFromANode();
};

ClassTest::ClassTest(){}

ClassTest::~ClassTest(){}

void ClassTest::test_addOneNode(){
    Graph graph;
    graph.addNode();
    QCOMPARE(graph.getNodeCount(), 1);
}

void ClassTest::test_addMoreThanOneNode(){
    Graph graph;
    graph.addNode();
    graph.addNode();
    graph.addNode();
    graph.addNode();
    QCOMPARE(graph.getNodeCount(), 4);
}

void ClassTest::test_addNodeWithAdjList(){
    Graph graph;
    std::vector<int> al = {1, 2, 3};
    graph.addNode(al);
    QCOMPARE(graph.getNodeCount(),1);
    QCOMPARE(static_cast<int>(graph.getAdjecencyList().size()),1);
    QCOMPARE(static_cast<int>(graph.getAdjecencyList()[0].size()),3);

    //check wether change in vector will trigger change in node's list
    al.push_back(4);
    QCOMPARE(static_cast<int>(graph.getAdjecencyList().size()),1);
    QCOMPARE(static_cast<int>(graph.getAdjecencyList()[0].size()),3);

    //add another node whith changed list
    graph.addNode(al);
    QCOMPARE(static_cast<int>(graph.getAdjecencyList().size()),2);
    QCOMPARE(static_cast<int>(graph.getAdjecencyList()[0].size()),3);
    QCOMPARE(static_cast<int>(graph.getAdjecencyList()[1].size()),4);
}

void ClassTest::test_removeNode(){
    Graph graph;
    graph.addNode();
    graph.addNode();
    graph.addNode();
    graph.removeNode(2);
    QCOMPARE(graph.getNodeCount(), 2);
}

void ClassTest::test_getNodesIndexes(){
    Graph graph;
    graph.addNode();
    graph.addNode();
    graph.addNode();
    std::vector<int> idxs = graph.getNodesIndexes();
    QCOMPARE(idxs[0], 0);
    QCOMPARE(idxs[1], 1);
    QCOMPARE(idxs[2], 2);

    graph.removeNode(1);
    idxs = graph.getNodesIndexes();
    QCOMPARE(idxs[0], 0);
    QCOMPARE(idxs[1], 2);
}

void ClassTest::test_changeIndex(){
    Graph graph;
    graph.addNode();
    graph.changeIndex(0,10);
    std::vector<int> idxs = graph.getNodesIndexes();
    QCOMPARE(idxs[0], 10);
}

void ClassTest::test_setSingleAdjecencyList(){
    Graph graph;
    graph.addNode();
    std::vector<int> al = {1, 2, 3};
    graph.setSingleAdjecencyList(0, al);
    QCOMPARE(graph.getAdjecencyList()[0], al);
}

void ClassTest::test_getSingleAdjcencyList(){
    Graph graph;
    graph.addNode();
    std::vector<int> al = {1, 2, 3};
    graph.setSingleAdjecencyList(0, al);
    QCOMPARE(graph.getSingleAdjecencyList(0), al);
}

void ClassTest::test_addAdjecentToANode(){
    Graph graph;
    graph.addNode();
    graph.addAdjecentToANode(5, 0);
    std::vector<int> al = {5};
    QCOMPARE(graph.getSingleAdjecencyList(0), al);
}

void ClassTest::test_removeAdjecentFromANode(){
    Graph graph;
    std::vector<int> al = {5};
    graph.addNode(al);
    graph.removeAdjecentFromANode(5,0);
    QVERIFY(graph.getSingleAdjecencyList(0).empty());
}


QTEST_APPLESS_MAIN(ClassTest)

#include "tst_classtest.moc"
