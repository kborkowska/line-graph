#include <QtTest>

// add necessary includes here

class ClassTest : public QObject
{
    Q_OBJECT

public:
    ClassTest();
    ~ClassTest();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();

};

ClassTest::ClassTest()
{

}

ClassTest::~ClassTest()
{

}

void ClassTest::initTestCase()
{

}

void ClassTest::cleanupTestCase()
{

}

void ClassTest::test_case1()
{

}

QTEST_APPLESS_MAIN(ClassTest)

#include "tst_classtest.moc"
