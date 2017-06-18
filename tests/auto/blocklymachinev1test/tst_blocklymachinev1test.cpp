#include <QtTest>

#include <QtTest>
#include <QTemporaryFile>
#include <QFile>

#include <memory>

// add necessary includes here

#include <blocklyFluidicMachineTranslator/blocklyfluidicmachinetranslator.h>

#include <commonmodel/plugininterface/pluginabstractfactory.h>

class BlocklyMachineV1Test : public QObject
{
    Q_OBJECT

private:
    void copyResourceFile(const QString & resourcePath, QTemporaryFile* file) throw(std::invalid_argument);
public:
    BlocklyMachineV1Test();
    ~BlocklyMachineV1Test();

private slots:
    void turbidostatMachineLayout();

};

BlocklyMachineV1Test::BlocklyMachineV1Test()
{

}

BlocklyMachineV1Test::~BlocklyMachineV1Test()
{

}

void BlocklyMachineV1Test::turbidostatMachineLayout()
{
    QTemporaryFile* tempFile = new QTemporaryFile();
    if (tempFile->open()) {
        try {
            copyResourceFile(":/machines/machines/turbidostat_test.json", tempFile);

            std::shared_ptr<PluginAbstractFactory> factory;
            BlocklyFluidicMachineTranslator translator(tempFile->fileName().toStdString(), factory);

            std::shared_ptr<FluidicMachineModel> model = translator.translateFile();

            std::string generatedMachine = model->getMachineGraph()->toString();
            qDebug() << "generated machine:";
            qDebug() << generatedMachine.c_str();

            std::string expectedMachine = "digraph{0[label=\"0_OPEN_CONTAINER\"];1[label=\"1_CLOSE_CONTAINER\"];3[label=\"3_OPEN_CONTAINER\"];2[label=\"2_UNIDIRECTIONAL_PUMP\"];2->3[label=\" direction=Unknow, cutted=0\"];0->1[label=\" direction=Unknow, cutted=0\"];1->2[label=\" direction=Unknow, cutted=0\"];}";
            qDebug() << "expected machine:";
            qDebug() << expectedMachine.c_str();

            QVERIFY2(generatedMachine.compare(expectedMachine) == 0, "generated machine and expected machine are not the same");

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

void BlocklyMachineV1Test::copyResourceFile(const QString & resourcePath, QTemporaryFile* tempFile) throw(std::invalid_argument) {
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

QTEST_APPLESS_MAIN(BlocklyMachineV1Test)

#include "tst_blocklymachinev1test.moc"
