#include <QString>
#include <QtTest>

#include <QtTest>
#include <QTemporaryFile>
#include <QFile>

#include <memory>

// add necessary includes here

#include <blocklyFluidicMachineTranslator/blocklyfluidicmachinetranslator.h>

#include <commonmodel/plugininterface/pluginabstractfactory.h>

class Blocklymachinev2testTest : public QObject
{
    Q_OBJECT

public:
    Blocklymachinev2testTest();

private:
    void copyResourceFile(const QString & resourcePath, QTemporaryFile* file) throw(std::invalid_argument);

private Q_SLOTS:
    void testCaseSImpleMachine();
    void testCaseComplexValve();
};

Blocklymachinev2testTest::Blocklymachinev2testTest()
{
}

void Blocklymachinev2testTest::testCaseSImpleMachine()
{
    QTemporaryFile* tempFile = new QTemporaryFile();
    if (tempFile->open()) {
        try {
            copyResourceFile(":/machines/machineV2simpleTest.json", tempFile);

            std::shared_ptr<PluginAbstractFactory> factory;
            BlocklyFluidicMachineTranslator translator(tempFile->fileName().toStdString(), factory);

            BlocklyFluidicMachineTranslator::ModelMappingTuple fluidicModelPair = translator.translateFile();

            std::shared_ptr<FluidicMachineModel> model = std::get<0>(fluidicModelPair);
            std::string generatedMachine = model->getMachineGraph()->toString();
            qDebug() << "generated machine:";
            qDebug() << generatedMachine.c_str();

            auto edge1 = model->getMachineGraph()->getTubeConnectedToPin(0,0);
            auto edge2 = model->getMachineGraph()->getTubeConnectedToPin(1,0);
            QVERIFY2(edge1->equals(*edge2), "Node 0 pin 0 and Node 1 pin 0 are not connected");

            edge1 = model->getMachineGraph()->getTubeConnectedToPin(1,1);
            edge2 = model->getMachineGraph()->getTubeConnectedToPin(2,0);
            QVERIFY2(edge1->equals(*edge2), "Node 1 pin 1 and Node 2 pin 0 are not connected");

            edge1 = model->getMachineGraph()->getTubeConnectedToPin(2,1);
            edge2 = model->getMachineGraph()->getTubeConnectedToPin(3,0);
            QVERIFY2(edge1->equals(*edge2), "Node 2 pin 1 and Node 3 pin 0 are not connected");

        } catch (std::exception & e) {
            delete tempFile;
            QFAIL(e.what());
        }
    } else {
        delete tempFile;
        QFAIL("imposible to create temporary file");
    }
    delete tempFile;
}

void Blocklymachinev2testTest::testCaseComplexValve() {
    QTemporaryFile* tempFile = new QTemporaryFile();
    if (tempFile->open()) {
        try {
            copyResourceFile(":/machines/machineV2complexValveTest.json", tempFile);

            std::shared_ptr<PluginAbstractFactory> factory;
            BlocklyFluidicMachineTranslator translator(tempFile->fileName().toStdString(), factory);

            BlocklyFluidicMachineTranslator::ModelMappingTuple fluidicModelPair = translator.translateFile();

            std::shared_ptr<FluidicMachineModel> model = std::get<0>(fluidicModelPair);
            std::string generatedMachine = model->getMachineGraph()->toString();
            qDebug() << "generated machine:";
            qDebug() << generatedMachine.c_str();

            auto edge1 = model->getMachineGraph()->getTubeConnectedToPin(0,0);
            auto edge2 = model->getMachineGraph()->getTubeConnectedToPin(1,0);
            QVERIFY2(edge1->equals(*edge2), "Node 0 pin 0 and Node 1 pin 0 are not connected");

            edge1 = model->getMachineGraph()->getTubeConnectedToPin(1,1);
            edge2 = model->getMachineGraph()->getTubeConnectedToPin(5,0);
            QVERIFY2(edge1->equals(*edge2), "Node 1 pin 1 and Node 5 pin 0 are not connected");

            edge1 = model->getMachineGraph()->getTubeConnectedToPin(2,0);
            edge2 = model->getMachineGraph()->getTubeConnectedToPin(3,0);
            QVERIFY2(edge1->equals(*edge2), "Node 2 pin 0 and Node 3 pin 0 are not connected");

            edge1 = model->getMachineGraph()->getTubeConnectedToPin(3,1);
            edge2 = model->getMachineGraph()->getTubeConnectedToPin(5,1);
            QVERIFY2(edge1->equals(*edge2), "Node 3 pin 1 and Node 5 pin 1 are not connected");

            edge1 = model->getMachineGraph()->getTubeConnectedToPin(5,2);
            edge2 = model->getMachineGraph()->getTubeConnectedToPin(6,0);
            QVERIFY2(edge1->equals(*edge2), "Node 5 pin 2 and Node 6 pin 0 are not connected");

            edge1 = model->getMachineGraph()->getTubeConnectedToPin(5,3);
            edge2 = model->getMachineGraph()->getTubeConnectedToPin(4,1);
            QVERIFY2(edge1->equals(*edge2), "Node 5 pin 3 and Node 4 pin 1 are not connected");

            edge1 = model->getMachineGraph()->getTubeConnectedToPin(5,4);
            edge2 = model->getMachineGraph()->getTubeConnectedToPin(4,0);
            QVERIFY2(edge1->equals(*edge2), "Node 5 pin 4 and Node 4 pin 0 are not connected");

        } catch (std::exception & e) {
            delete tempFile;
            QFAIL(e.what());
        }
    } else {
        delete tempFile;
        QFAIL("imposible to create temporary file");
    }
    delete tempFile;
}

void Blocklymachinev2testTest::copyResourceFile(const QString & resourcePath, QTemporaryFile* tempFile) throw(std::invalid_argument) {
    QFile resourceFile(resourcePath);
    if(!resourceFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw(std::invalid_argument("imposible to open" + resourcePath.toStdString()));
    }

    QTextStream out(tempFile);

    QTextStream in(&resourceFile);
    while (!in.atEnd()) {
        QString line = in.readLine();
        out << line;
    }
    out.flush();
}

QTEST_APPLESS_MAIN(Blocklymachinev2testTest)

#include "tst_blocklymachinev2testtest.moc"
