#include <QtTest>

// add necessary includes here

class blocklyMachineV1Test : public QObject
{
    Q_OBJECT

public:
    blocklyMachineV1Test();
    ~blocklyMachineV1Test();

private slots:
    void test_case1();

};

blocklyMachineV1Test::blocklyMachineV1Test()
{

}

blocklyMachineV1Test::~blocklyMachineV1Test()
{

}

void blocklyMachineV1Test::test_case1()
{

}

QTEST_APPLESS_MAIN(blocklyMachineV1Test)

#include "tst_blocklymachinev1test.moc"
