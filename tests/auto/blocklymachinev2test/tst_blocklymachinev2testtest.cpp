#include <pythonPlugins/environment/PluginFileLoader.h>
#include <pythonPlugins/environment/PythonEnvironment.h>
#include <pythonPlugins/actuators/pythonpluginabstractfactory.h>

#include <QString>
#include <QtTest>

#include <QtTest>
#include <QTemporaryFile>
#include <QFile>

#include <memory>
#include <sstream>

// add necessary includes here

#include <blocklyFluidicMachineTranslator/blocklyfluidicmachinetranslator.h>

#include <commonmodel/plugininterface/pluginabstractfactory.h>

#include "debugcommandsender.h"
#include "stringpluginfactory.h"

class Blocklymachinev2testTest : public QObject
{
    Q_OBJECT

public:
    Blocklymachinev2testTest();

private:
    void copyResourceFile(const QString & resourcePath, QTemporaryFile* file) throw(std::invalid_argument);
    std::string ttToString(const ValveNode::TruthTable & table);
    bool checkSolutions(const std::string & generated, const std::string & expected);

private Q_SLOTS:
    void testCaseSImpleMachine();
    void testCaseComplexValve();
    void testCaseEvoprogMachine();
    void testCaseTwinValve();
    void testCaseEvoprogTwin();
    void testCaseEvoprogTwinTT();
    void testCaseDirectionTest();
    void testCaseEvoprogTwinNoCleanStructure();
    void testCaseEvoprogTwinNoClean();

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

void Blocklymachinev2testTest::testCaseEvoprogMachine() {
    std::shared_ptr<DebugCommandSender> com = std::make_shared<DebugCommandSender>();

    QTemporaryFile* tempFile = new QTemporaryFile();
    if (tempFile->open()) {
        try {
            copyResourceFile(":/machines/evoprogCompleteMachineV2.json", tempFile);

            std::shared_ptr<PluginAbstractFactory> factory = std::make_shared<PythonPluginAbstractFactory>(com);
            com->connect();

            BlocklyFluidicMachineTranslator translator(tempFile->fileName().toStdString(), factory);
            BlocklyFluidicMachineTranslator::ModelMappingTuple fluidicModelPair = translator.translateFile();
            std::shared_ptr<FluidicMachineModel> model = std::get<0>(fluidicModelPair);

            std::string generatedMachine = model->getMachineGraph()->toString();
            qDebug() << "generated machine:";
            qDebug() << generatedMachine.c_str();

            PluginFileLoader::setPluginDir("X:/blockly_fluidicMachine_translator/blocklyFluidicMachineTranslatorTest/tests/auto/blocklymachinev2test/plugins");
            PythonEnvironment::GetInstance()->initEnvironment("X:/pluginScripts/interfacePlugins");

            PrologExecutor::createEngine(std::string(QTest::currentAppName()));

            std::unordered_map<std::string, int> cId(translator.getVariableIdMap());

            int c1 = cId["mediaB"];
            int c2 = cId["chemoB"];
            int c3 = cId["wasteB"];

            qDebug() << "c1" << c1;
            qDebug() << "c2" << c2;
            qDebug() << "c3" << c3;

            model->setContinuousFlow({c1, c2, c3}, 300 * units::ml/units::hr);
            model->processFlows({});

            std::string expected = "pPump 8;P 0 2560.0;pPump 7;pPump 2;pPump 5;pPump 6;M 1 3;M 4 2;";
            std::string generated = com->getStr();

            qDebug() << "generated: " << generated.c_str();
            qDebug() << "expected: " << expected.c_str();

            QVERIFY2(expected.compare(generated) == 0, "expected and generated outputs are not the same check debug for more information");

            /*model->stopContinuousFlow({c1, c2, c3});

            c1 = cId["mediaB"];
            c2 = cId["chemoB"];
            c3 = cId["wasteB"];

            qDebug() << "c1" << c1;
            qDebug() << "c2" << c2;
            qDebug() << "c3" << c3;

            model->setContinuousFlow({c1, c2, c3}, 1000 * units::ml/units::hr);
            model->processFlows();

            expected = "pPump 8;P 0 2560.0;pPump 7;pPump 2;pPump 5;pPump 6;M 1 3;M 4 2;";
            generated = com->getStr();

            qDebug() << "generated: " << generated.c_str();
            qDebug() << "expected: " << expected.c_str();

            QVERIFY2(expected.compare(generated) == 0, "expected and generated outputs are not the same check debug for more information");*/

        } catch (std::exception & e) {
            delete tempFile;

            PythonEnvironment::GetInstance()->finishEnvironment();
            PrologExecutor::destoryEngine();
            com->disconnect();

            QFAIL(e.what());
        }
    } else {
        delete tempFile;
        QFAIL("imposible to create temporary file");
    }

    delete tempFile;

    PythonEnvironment::GetInstance()->finishEnvironment();
    PrologExecutor::destoryEngine();
    com->disconnect();
}

void Blocklymachinev2testTest::testCaseTwinValve() {
    std::shared_ptr<DebugCommandSender> com = std::make_shared<DebugCommandSender>();

    QTemporaryFile* tempFile = new QTemporaryFile();
    if (tempFile->open()) {
        try {
            copyResourceFile(":/machines/twinValveTest2.json", tempFile);

            std::shared_ptr<PluginAbstractFactory> factory = std::make_shared<PythonPluginAbstractFactory>(com);
            com->connect();

            BlocklyFluidicMachineTranslator translator(tempFile->fileName().toStdString(), factory);
            BlocklyFluidicMachineTranslator::ModelMappingTuple fluidicModelPair = translator.translateFile();
            std::shared_ptr<FluidicMachineModel> model = std::get<0>(fluidicModelPair);

            std::string generatedMachine = model->getMachineGraph()->toString();
            qDebug() << "generated machine:";
            qDebug() << generatedMachine.c_str();

            PluginFileLoader::setPluginDir("X:/blockly_fluidicMachine_translator/blocklyFluidicMachineTranslatorTest/tests/auto/blocklymachinev2test/plugins");
            PythonEnvironment::GetInstance()->initEnvironment("X:/pluginScripts/interfacePlugins");

            PrologExecutor::createEngine(std::string(QTest::currentAppName()));

            std::unordered_map<std::string, int> cId(translator.getVariableIdMap());

            int c1 = cId["c1"];
            int c2 = cId["c2"];
            int c3 = cId["c4"];

            model->setContinuousFlow({c1, c2, c3}, 300 * units::ml/units::hr);
            model->processFlows({});

            std::string expected = "P 0 768.0;pPump 2;M 1 3;M 3 3;";
            std::string generated = com->getStr();

            qDebug() << "generated: " << generated.c_str();
            qDebug() << "expected: " << expected.c_str();

            QVERIFY2(expected.compare(generated) == 0, "expected and generated outputs are not the same check debug for more information");

            model->stopContinuousFlow({c1, c2, c3});

            c1 = cId["c1"];
            c2 = cId["c2"];
            c3 = cId["c3"];

            model->setContinuousFlow({c1, c2, c3}, 300 * units::ml/units::hr);
            try {
                model->processFlows({});
                QFAIL("flow c1->c2->c3 is impossible");
            } catch (std::runtime_error & e) {
                QVERIFY(true);
            } catch (std::exception & e) {
                QFAIL(e.what());
            }

        } catch (std::exception & e) {
            delete tempFile;

            PythonEnvironment::GetInstance()->finishEnvironment();
            PrologExecutor::destoryEngine();
            com->disconnect();

            QFAIL(e.what());
        }
    } else {
        delete tempFile;
        QFAIL("imposible to create temporary file");
    }

    delete tempFile;

    PythonEnvironment::GetInstance()->finishEnvironment();
    PrologExecutor::destoryEngine();
    com->disconnect();
}

void Blocklymachinev2testTest::testCaseEvoprogTwin() {
    std::shared_ptr<DebugCommandSender> com = std::make_shared<DebugCommandSender>();

    QTemporaryFile* tempFile = new QTemporaryFile();
    if (tempFile->open()) {
        try {
            copyResourceFile(":/machines/TWIN_MACHINE_V5.json", tempFile);

            std::shared_ptr<PythonPluginAbstractFactory> factory = std::make_shared<PythonPluginAbstractFactory>(com);
            com->connect();

            BlocklyFluidicMachineTranslator translator(tempFile->fileName().toStdString(), factory);
            BlocklyFluidicMachineTranslator::ModelMappingTuple fluidicModelPair = translator.translateFile();
            std::shared_ptr<FluidicMachineModel> model = std::get<0>(fluidicModelPair);

            std::string generatedMachine = model->getMachineGraph()->toString();
            qDebug() << "generated machine:";
            qDebug() << generatedMachine.c_str();

            PluginFileLoader::setPluginDir("X:/blockly_fluidicMachine_translator/blocklyFluidicMachineTranslatorTest/tests/auto/blocklymachinev2test/plugins");
            PythonEnvironment::GetInstance()->initEnvironment("X:/pluginScripts/interfacePlugins");

            PrologExecutor::createEngine(std::string(QTest::currentAppName()));

            std::unordered_map<std::string, int> cId(translator.getVariableIdMap());

            model->setContinuousFlow({cId["MEDIA_A"], cId["WASTE_A"]}, 300 * units::ml/units::hr);
            model->setContinuousFlow({cId["MEDIA_B"], cId["WASTE_B"]}, 300 * units::ml/units::hr);
            model->processFlows({});

            std::string expected = "";
            std::string generated = com->getStr();

            qDebug() << "generated: " << generated.c_str();
            qDebug() << "expected: " << expected.c_str();

            QVERIFY2(expected.compare(generated) == 0, "expected and generated outputs are not the same check debug for more information");
        } catch (std::exception & e) {
            delete tempFile;

            PythonEnvironment::GetInstance()->finishEnvironment();
            PrologExecutor::destoryEngine();
            com->disconnect();

            QFAIL(e.what());
        }
    } else {
        delete tempFile;
        QFAIL("imposible to create temporary file");
    }

    delete tempFile;

    PythonEnvironment::GetInstance()->finishEnvironment();
    PrologExecutor::destoryEngine();
    com->disconnect();
}

void Blocklymachinev2testTest::testCaseEvoprogTwinTT() {
    QTemporaryFile* tempFile = new QTemporaryFile();
    if (tempFile->open()) {
        try {
            copyResourceFile(":/machines/TWIN_MACHINE_V5.json", tempFile);

            std::shared_ptr<StringPluginFactory> factory = std::make_shared<StringPluginFactory>();

            BlocklyFluidicMachineTranslator translator(tempFile->fileName().toStdString(), factory);
            BlocklyFluidicMachineTranslator::ModelMappingTuple fluidicModelPair = translator.translateFile();
            std::shared_ptr<FluidicMachineModel> model = std::get<0>(fluidicModelPair);

            std::string generatedMachine = model->getMachineGraph()->toString();
            qDebug() << "generated machine:";
            qDebug() << generatedMachine.c_str();

            std::unordered_map<std::string, int> cId(translator.getVariableIdMap());
            qDebug() << "VA_A :" << ttToString(model->getMachineGraph()->getValve(cId["VA_A"])->getTruthTable()).c_str();
            qDebug() << "VA_B :" << ttToString(model->getMachineGraph()->getValve(cId["VA_B"])->getTruthTable()).c_str();

            qDebug() << "VB_B :" << ttToString(model->getMachineGraph()->getValve(cId["VB_B"])->getTruthTable()).c_str();
            qDebug() << "VB_B :" << ttToString(model->getMachineGraph()->getValve(cId["VB_B"])->getTruthTable()).c_str();

            qDebug() << "VC_A :" << ttToString(model->getMachineGraph()->getValve(cId["VC_A"])->getTruthTable()).c_str();
            qDebug() << "VC_B :" << ttToString(model->getMachineGraph()->getValve(cId["VC_B"])->getTruthTable()).c_str();
            qDebug() << "VC_C :" << ttToString(model->getMachineGraph()->getValve(cId["VC_C"])->getTruthTable()).c_str();

            qDebug() << "V_CLEAN :" << ttToString(model->getMachineGraph()->getValve(cId["V_CLEAN"])->getTruthTable()).c_str();

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

void Blocklymachinev2testTest::testCaseDirectionTest() {
    std::shared_ptr<DebugCommandSender> com = std::make_shared<DebugCommandSender>();

    QTemporaryFile* tempFile = new QTemporaryFile();
    if (tempFile->open()) {
        try {
            copyResourceFile(":/machines/directionPortsTest.json", tempFile);

            std::shared_ptr<PluginAbstractFactory> factory = std::make_shared<PythonPluginAbstractFactory>(com);

            BlocklyFluidicMachineTranslator translator(tempFile->fileName().toStdString(), factory);
            BlocklyFluidicMachineTranslator::ModelMappingTuple fluidicModelPair = translator.translateFile();
            std::shared_ptr<FluidicMachineModel> model = std::get<0>(fluidicModelPair);

            std::string generatedMachine = model->getMachineGraph()->toString();
            qDebug() << "generated machine:";
            qDebug() << generatedMachine.c_str();

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

void Blocklymachinev2testTest::testCaseEvoprogTwinNoCleanStructure() {
    QTemporaryFile* tempFile = new QTemporaryFile();
    if (tempFile->open()) {
        try {
            copyResourceFile(":/machines/EVOPROG_NO_CLEANING.json", tempFile);

            std::shared_ptr<PythonPluginAbstractFactory> factory = NULL;

            BlocklyFluidicMachineTranslator translator(tempFile->fileName().toStdString(), factory);
            BlocklyFluidicMachineTranslator::ModelMappingTuple fluidicModelPair = translator.translateFile();
            std::shared_ptr<FluidicMachineModel> model = std::get<0>(fluidicModelPair);

            std::unordered_map<std::string, int> cId(translator.getVariableIdMap());

            const std::shared_ptr<const MachineGraph> machine = model->getMachineGraph();
            std::string generatedMachine = machine->toString();
            qDebug() << "generated machine:";
            qDebug() << generatedMachine.c_str();

            int port1 = 0;
            int port2 = 0;
            auto edge1 = machine->getTubeConnectedToPin(cId["media_A"],port1);
            auto edge2 = machine->getTubeConnectedToPin(cId["PA"],port2);
            QVERIFY2(edge1->equals(*edge2.get()), std::string(edge1->toText() + ":" + std::to_string(port1) + " and " +
                                                              edge2->toText() + ":" + std::to_string(port2) + " are not the same").c_str());

            port1 = 1;
            port2 = 0;
            edge1 = machine->getTubeConnectedToPin(cId["PA"],port1);
            edge2 = machine->getTubeConnectedToPin(cId["VA_A"],port2);
            QVERIFY2(edge1->equals(*edge2.get()), std::string(edge1->toText() + ":" + std::to_string(port1) + " and " +
                                                              edge2->toText() + ":" + std::to_string(port2) + " are not the same").c_str());

            port1 = 1;
            port2 = 0;
            edge1 = machine->getTubeConnectedToPin(cId["VA_A"],port1);
            edge2 = machine->getTubeConnectedToPin(cId["chemo_A"],port2);
            QVERIFY2(edge1->equals(*edge2.get()), std::string(edge1->toText() + ":" + std::to_string(port1) + " and " +
                                                              edge2->toText() + ":" + std::to_string(port2) + " are not the same").c_str());

            port1 = 1;
            port2 = 0;
            edge1 = machine->getTubeConnectedToPin(cId["chemo_A"],port1);
            edge2 = machine->getTubeConnectedToPin(cId["VA_B"],port2);
            QVERIFY2(edge1->equals(*edge2.get()), std::string(edge1->toText() + ":" + std::to_string(port1) + " and " +
                                                              edge2->toText() + ":" + std::to_string(port2) + " are not the same").c_str());

            port1 = 1;
            port2 = 0;
            edge1 = machine->getTubeConnectedToPin(cId["VA_B"],port1);
            edge2 = machine->getTubeConnectedToPin(cId["VC_A"],port2);
            QVERIFY2(edge1->equals(*edge2.get()), std::string(edge1->toText() + ":" + std::to_string(port1) + " and " +
                                                              edge2->toText() + ":" + std::to_string(port2) + " are not the same").c_str());

            port1 = 1;
            port2 = 0;
            edge1 = machine->getTubeConnectedToPin(cId["VC_A"],port1);
            edge2 = machine->getTubeConnectedToPin(cId["waste_A"],port2);
            QVERIFY2(edge1->equals(*edge2.get()), std::string(edge1->toText() + ":" + std::to_string(port1) + " and " +
                                                              edge2->toText() + ":" + std::to_string(port2) + " are not the same").c_str());

            port1 = 2;
            port2 = 0;
            edge1 = machine->getTubeConnectedToPin(cId["VC_A"],port1);
            edge2 = machine->getTubeConnectedToPin(cId["cellstat"],port2);
            QVERIFY2(edge1->equals(*edge2.get()), std::string(edge1->toText() + ":" + std::to_string(port1) + " and " +
                                                              edge2->toText() + ":" + std::to_string(port2) + " are not the same").c_str());

            port1 = 2;
            port2 = 0;
            edge1 = machine->getTubeConnectedToPin(cId["cellstat"],port1);
            edge2 = machine->getTubeConnectedToPin(cId["VC_C"],port2);
            QVERIFY2(edge1->equals(*edge2.get()), std::string(edge1->toText() + ":" + std::to_string(port1) + " and " +
                                                              edge2->toText() + ":" + std::to_string(port2) + " are not the same").c_str());

            port1 = 1;
            port2 = 0;
            edge1 = machine->getTubeConnectedToPin(cId["VC_C"],port1);
            edge2 = machine->getTubeConnectedToPin(cId["waste_C"],port2);
            QVERIFY2(edge1->equals(*edge2.get()), std::string(edge1->toText() + ":" + std::to_string(port1) + " and " +
                                                              edge2->toText() + ":" + std::to_string(port2) + " are not the same").c_str());

            port1 = 0;
            port2 = 0;
            edge1 = machine->getTubeConnectedToPin(cId["media_B"],port1);
            edge2 = machine->getTubeConnectedToPin(cId["PB"],port2);
            QVERIFY2(edge1->equals(*edge2.get()), std::string(edge1->toText() + ":" + std::to_string(port1) + " and " +
                                                              edge2->toText() + ":" + std::to_string(port2) + " are not the same").c_str());

            port1 = 1;
            port2 = 0;
            edge1 = machine->getTubeConnectedToPin(cId["PB"],port1);
            edge2 = machine->getTubeConnectedToPin(cId["VB_A"],port2);
            QVERIFY2(edge1->equals(*edge2.get()), std::string(edge1->toText() + ":" + std::to_string(port1) + " and " +
                                                              edge2->toText() + ":" + std::to_string(port2) + " are not the same").c_str());

            port1 = 1;
            port2 = 0;
            edge1 = machine->getTubeConnectedToPin(cId["VB_A"],port1);
            edge2 = machine->getTubeConnectedToPin(cId["chemo_B"],port2);
            QVERIFY2(edge1->equals(*edge2.get()), std::string(edge1->toText() + ":" + std::to_string(port1) + " and " +
                                                              edge2->toText() + ":" + std::to_string(port2) + " are not the same").c_str());

            port1 = 1;
            port2 = 0;
            edge1 = machine->getTubeConnectedToPin(cId["chemo_B"],port1);
            edge2 = machine->getTubeConnectedToPin(cId["VB_B"],port2);
            QVERIFY2(edge1->equals(*edge2.get()), std::string(edge1->toText() + ":" + std::to_string(port1) + " and " +
                                                              edge2->toText() + ":" + std::to_string(port2) + " are not the same").c_str());

            port1 = 1;
            port2 = 0;
            edge1 = machine->getTubeConnectedToPin(cId["VB_B"],port1);
            edge2 = machine->getTubeConnectedToPin(cId["VC_B"],port2);
            QVERIFY2(edge1->equals(*edge2.get()), std::string(edge1->toText() + ":" + std::to_string(port1) + " and " +
                                                              edge2->toText() + ":" + std::to_string(port2) + " are not the same").c_str());

            port1 = 1;
            port2 = 0;
            edge1 = machine->getTubeConnectedToPin(cId["VC_B"],port1);
            edge2 = machine->getTubeConnectedToPin(cId["waste_B"],port2);
            QVERIFY2(edge1->equals(*edge2.get()), std::string(edge1->toText() + ":" + std::to_string(port1) + " and " +
                                                              edge2->toText() + ":" + std::to_string(port2) + " are not the same").c_str());

            port1 = 2;
            port2 = 1;
            edge1 = machine->getTubeConnectedToPin(cId["VC_B"],port1);
            edge2 = machine->getTubeConnectedToPin(cId["cellstat"],port2);
            QVERIFY2(edge1->equals(*edge2.get()), std::string(edge1->toText() + ":" + std::to_string(port1) + " and " +
                                                              edge2->toText() + ":" + std::to_string(port2) + " are not the same").c_str());

            const std::vector<std::unordered_set<int>> & twins = machine->getTwinsValves();

            bool finded = false;
            for(const std::unordered_set<int> & actualSet : twins) {

                if (actualSet.find(cId["VA_A"]) != actualSet.end() &&
                    actualSet.find(cId["VA_B"]) != actualSet.end())
                {
                    finded = true;
                    break;
                }
            }
            QVERIFY2(finded, "VA_A and VA_B are not twins");

            finded = false;
            for(const std::unordered_set<int> & set : twins) {

                if (set.find(cId["VB_A"]) != set.end() &&
                    set.find(cId["VB_B"]) != set.end())
                {
                    finded = true;
                    break;
                }
            }
            QVERIFY2(finded, "VB_A and VB_B are not twins");

            finded = false;
            for(const std::unordered_set<int> & set : twins) {

                if (set.find(cId["VC_A"]) != set.end() &&
                    set.find(cId["VC_B"]) != set.end() &&
                    set.find(cId["VC_C"]) != set.end())
                {
                    finded = true;
                    break;
                }
            }
            QVERIFY2(finded, "VC_A, VC_B and VC_C are not twins");

            auto table = machine->getValve(cId["VA_A"])->getTruthTable();
            auto pos = table[0];
            QVERIFY2(pos.empty(), "pos 0 of VA_A is not correct");

            table = machine->getValve(cId["VA_A"])->getTruthTable();
            pos = table[3];
            QVERIFY2(pos[0].find(0) != pos[0].end() && pos[0].find(1) != pos[0].end(), "pos 3 of VA_A is not correct");

            table = machine->getValve(cId["VA_B"])->getTruthTable();
            pos = table[0];
            QVERIFY2(pos.empty(), "pos 0 of VA_B is not correct");

            table = machine->getValve(cId["VA_B"])->getTruthTable();
            pos = table[3];
            QVERIFY2(pos[0].find(0) != pos[0].end() && pos[0].find(1) != pos[0].end(), "pos 3 of VA_B is not correct");

            table = machine->getValve(cId["VB_A"])->getTruthTable();
            pos = table[0];
            QVERIFY2(pos.empty(), "pos 0 of VB_A is not correct");

            table = machine->getValve(cId["VB_A"])->getTruthTable();
            pos = table[3];
            QVERIFY2(pos[0].find(0) != pos[0].end() && pos[0].find(1) != pos[0].end(), "pos 3 of VB_A is not correct");

            table = machine->getValve(cId["VB_B"])->getTruthTable();
            pos = table[0];
            QVERIFY2(pos.empty(), "pos 0 of VB_B is not correct");

            table = machine->getValve(cId["VB_B"])->getTruthTable();
            pos = table[3];
            QVERIFY2(pos[0].find(0) != pos[0].end() && pos[0].find(1) != pos[0].end(), "pos 3 of VB_B is not correct");

            table = machine->getValve(cId["VC_A"])->getTruthTable();
            pos = table[0];
            QVERIFY2(pos.empty(), "pos 0 of VC_A is not correct");

            table = machine->getValve(cId["VC_A"])->getTruthTable();
            pos = table[1];
            QVERIFY2(pos[0].find(0) != pos[0].end() && pos[0].find(2) != pos[0].end(), "pos 1 of VC_A is not correct");

            table = machine->getValve(cId["VC_A"])->getTruthTable();
            pos = table[2];
            QVERIFY2(pos[0].find(0) != pos[0].end() && pos[0].find(1) != pos[0].end(), "pos 2 of VC_A is not correct");

            table = machine->getValve(cId["VC_A"])->getTruthTable();
            pos = table[4];
            QVERIFY2(pos[0].find(0) != pos[0].end() && pos[0].find(1) != pos[0].end(), "pos 4 of VC_A is not correct");

            table = machine->getValve(cId["VC_B"])->getTruthTable();
            pos = table[0];
            QVERIFY2(pos.empty(), "pos 0 of VC_B is not correct");

            table = machine->getValve(cId["VC_B"])->getTruthTable();
            pos = table[1];
            QVERIFY2(pos[0].find(0) != pos[0].end() && pos[0].find(1) != pos[0].end(), "pos 1 of VC_B is not correct");

            table = machine->getValve(cId["VC_B"])->getTruthTable();
            pos = table[2];
            QVERIFY2(pos[0].find(0) != pos[0].end() && pos[0].find(2) != pos[0].end(), "pos 2 of VC_B is not correct");

            table = machine->getValve(cId["VC_B"])->getTruthTable();
            pos = table[4];
            QVERIFY2(pos[0].find(0) != pos[0].end() && pos[0].find(1) != pos[0].end(), "pos 4 of VC_B is not correct");

            table = machine->getValve(cId["VC_C"])->getTruthTable();
            pos = table[0];
            QVERIFY2(pos.empty(), "pos 0 of VC_C is not correct");

            table = machine->getValve(cId["VC_C"])->getTruthTable();
            pos = table[1];
            QVERIFY2(pos[0].find(0) != pos[0].end() && pos[0].find(1) != pos[0].end(), "pos 1 of VC_C is not correct");

            table = machine->getValve(cId["VC_C"])->getTruthTable();
            pos = table[2];
            QVERIFY2(pos[0].find(0) != pos[0].end() && pos[0].find(1) != pos[0].end(), "pos 2 of VC_C is not correct");

            table = machine->getValve(cId["VC_C"])->getTruthTable();
            pos = table[4];
            QVERIFY2(pos[0].find(0) != pos[0].end() && pos[0].find(1) != pos[0].end(), "pos 4 of VC_C is not correct");

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

void Blocklymachinev2testTest::testCaseEvoprogTwinNoClean() {
    std::shared_ptr<DebugCommandSender> com = std::make_shared<DebugCommandSender>();

    QTemporaryFile* tempFile = new QTemporaryFile();
    if (tempFile->open()) {
        try {
            copyResourceFile(":/machines/EVOPROG_NO_CLEANING.json", tempFile);

            std::shared_ptr<PythonPluginAbstractFactory> factory = std::make_shared<PythonPluginAbstractFactory>(com);
            com->connect();

            BlocklyFluidicMachineTranslator translator(tempFile->fileName().toStdString(), factory);
            BlocklyFluidicMachineTranslator::ModelMappingTuple fluidicModelPair = translator.translateFile();
            std::shared_ptr<FluidicMachineModel> model = std::get<0>(fluidicModelPair);

            std::string generatedMachine = model->getMachineGraph()->toString();
            qDebug() << "generated machine:";
            qDebug() << generatedMachine.c_str();

            PluginFileLoader::setPluginDir("X:/blockly_fluidicMachine_translator/blocklyFluidicMachineTranslatorTest/tests/auto/blocklymachinev2test/plugins");
            PythonEnvironment::GetInstance()->initEnvironment("X:/pluginScripts/interfacePlugins");

            PrologExecutor::createEngine(std::string(QTest::currentAppName()));

            std::unordered_map<std::string, int> cId(translator.getVariableIdMap());

            qDebug() << "empty";

            model->processFlows({});

            std::string expected = "PUMP 17 0.0 0;PUMP 16 0.0 0;MOVE 62 0;MOVE 31 0;MOVE 31 0;MOVE 32 0;MOVE 32 0;MOVE 62 0;MOVE 62 0;";
            std::string generated = com->getStr();

            qDebug() << "generated: " << generated.c_str();
            qDebug() << "expected: " << expected.c_str();

            QVERIFY2(checkSolutions(generated, expected), "expected and generated outputs are not the same check debug for more information");
            com->clear();

            qDebug() << "media_A->chemo_A->cellstat->waste_C";

            model->setContinuousFlow({cId["media_A"], cId["chemo_A"], cId["cellstat"], cId["waste_C"]}, 300 * units::ml/units::hr);
            model->processFlows({});

            expected = "PUMP 17 8.33333333333e-05 1;PUMP 16 0.0 0;MOVE 62 1;MOVE 31 3;MOVE 31 3;MOVE 32 0;MOVE 32 0;MOVE 62 1;MOVE 62 1;";
            generated = com->getStr();

            qDebug() << "generated: " << generated.c_str();
            qDebug() << "expected: " << expected.c_str();

            QVERIFY2(checkSolutions(generated, expected), "expected and generated outputs are not the same check debug for more information");
            com->clear();

            qDebug() << "media_B->chemo_B->cellstat->waste_C";

            model->stopContinuousFlow({cId["media_A"], cId["chemo_A"], cId["cellstat"], cId["waste_C"]});
            model->setContinuousFlow({cId["media_B"], cId["chemo_B"], cId["cellstat"], cId["waste_C"]}, 300 * units::ml/units::hr);
            model->processFlows({});

            expected = "PUMP 17 0.0 0;PUMP 16 8.33333333333e-05 1;MOVE 62 2;MOVE 31 0;MOVE 31 0;MOVE 32 3;MOVE 32 3;MOVE 62 2;MOVE 62 2;";
            generated = com->getStr();

            qDebug() << "generated: " << generated.c_str();
            qDebug() << "expected: " << expected.c_str();

            QVERIFY2(checkSolutions(generated, expected), "expected and generated outputs are not the same check debug for more information");

            com->clear();

            qDebug() << "media_A->chemo_A->waste_A";
            qDebug() << "media_B->chemo_B->cellstat->waste_C";

            model->setContinuousFlow({cId["media_A"], cId["chemo_A"], cId["waste_A"]}, 300 * units::ml/units::hr);
            model->processFlows({});

            expected = "PUMP 17 8.33333333333e-05 1;PUMP 16 8.33333333333e-05 1;MOVE 62 2;MOVE 31 3;MOVE 31 3;MOVE 32 3;MOVE 32 3;MOVE 62 2;MOVE 62 2;";
            generated = com->getStr();

            qDebug() << "generated: " << generated.c_str();
            qDebug() << "expected: " << expected.c_str();

            QVERIFY2(checkSolutions(generated, expected), "expected and generated outputs are not the same check debug for more information");

            com->clear();

            qDebug() << "media_A->chemo_A->cellstat->waste_C";
            qDebug() << "media_B->chemo_B->waste_B";

            model->stopContinuousFlow({cId["media_B"], cId["chemo_B"], cId["cellstat"], cId["waste_C"]});
            model->stopContinuousFlow({cId["media_A"], cId["chemo_A"], cId["waste_A"]});

            model->setContinuousFlow({cId["media_A"], cId["chemo_A"], cId["cellstat"], cId["waste_C"]}, 300 * units::ml/units::hr);
            model->setContinuousFlow({cId["media_B"], cId["chemo_B"], cId["waste_B"]}, 300 * units::ml/units::hr);

            model->processFlows({});

            expected = "PUMP 17 8.33333333333e-05 1;PUMP 16 8.33333333333e-05 1;MOVE 62 1;MOVE 31 3;MOVE 31 3;MOVE 32 3;MOVE 32 3;MOVE 62 1;MOVE 62 1;";
            generated = com->getStr();

            qDebug() << "generated: " << generated.c_str();
            qDebug() << "expected: " << expected.c_str();

            QVERIFY2(checkSolutions(generated, expected), "expected and generated outputs are not the same check debug for more information");

            com->clear();

            qDebug() << "media_A->chemo_A->waste_A";
            qDebug() << "media_B->chemo_B->waste_B";

            model->stopContinuousFlow({cId["media_A"], cId["chemo_A"], cId["cellstat"], cId["waste_C"]});

            model->setContinuousFlow({cId["media_A"], cId["chemo_A"], cId["waste_A"]}, 300 * units::ml/units::hr);

            model->processFlows({});

            expected = "PUMP 17 8.33333333333e-05 1;PUMP 16 8.33333333333e-05 1;MOVE 62 4;MOVE 31 3;MOVE 31 3;MOVE 32 3;MOVE 32 3;MOVE 62 4;MOVE 62 4;";
            generated = com->getStr();

            qDebug() << "generated: " << generated.c_str();
            qDebug() << "expected: " << expected.c_str();

            QVERIFY2(checkSolutions(generated, expected), "expected and generated outputs are not the same check debug for more information");

            com->clear();

            qDebug() << "impossible";
            qDebug() << "media_A->chemo_A->cellstat->waste_C";
            qDebug() << "media_B->chemo_B->cellstat->waste_C";

            model->stopContinuousFlow({cId["media_A"], cId["chemo_A"], cId["waste_A"]});
            model->stopContinuousFlow({cId["media_B"], cId["chemo_B"], cId["waste_B"]});

            model->setContinuousFlow({cId["media_A"], cId["chemo_A"], cId["cellstat"], cId["waste_C"]}, 300 * units::ml/units::hr);
            model->setContinuousFlow({cId["media_B"], cId["chemo_B"], cId["cellstat"], cId["waste_C"]}, 300 * units::ml/units::hr);

            try {
                model->processFlows({});
                QFAIL("impossible flow has been made");
            } catch(std::exception & e) {
                qDebug() << e.what();
            }
        } catch (std::exception & e) {
            delete tempFile;

            PythonEnvironment::GetInstance()->finishEnvironment();
            PrologExecutor::destoryEngine();
            com->disconnect();

            QFAIL(e.what());
        }
    } else {
        delete tempFile;
        QFAIL("imposible to create temporary file");
    }

    delete tempFile;

    PythonEnvironment::GetInstance()->finishEnvironment();
    PrologExecutor::destoryEngine();
    com->disconnect();
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

std::string Blocklymachinev2testTest::ttToString(const ValveNode::TruthTable & table) {
    std::stringstream stream;

    stream << "[";
    for(const auto & actualPair : table) {
        stream << actualPair.first << ": [";

        for(const std::unordered_set<int> & posConnected : actualPair.second) {
            stream << "[";
            for(int port : posConnected) {
                stream << port << ",";
            }
            stream << "]";
        }
        stream << "];";
    }
    stream << "]";

    return stream.str();
}

bool Blocklymachinev2testTest::checkSolutions(const std::string & generated, const std::string & expected) {
    std::vector<std::string> generatedTokens;
    Utils::tokenize(generated, generatedTokens, ";");

    std::vector<std::string> expectedTokens;
    Utils::tokenize(expected, expectedTokens, ";");

    std::unordered_set<std::string> generatedSet(generatedTokens.begin(), generatedTokens.end());
    std::unordered_set<std::string> expectedSet(expectedTokens.begin(), expectedTokens.end());
    return (generatedSet == expectedSet);
}

QTEST_APPLESS_MAIN(Blocklymachinev2testTest)

#include "tst_blocklymachinev2testtest.moc"
