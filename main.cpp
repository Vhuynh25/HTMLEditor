#include "mainwindow.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("HTML Editor Project");
    QCoreApplication::setApplicationName("HTML Editor");
    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::applicationName());
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file","The file to open");
    parser.process(a);
    MainWindow w;
    if (!parser.positionalArguments().isEmpty()){
        w.load_File(parser.positionalArguments().first());
    }
    w.show();

    return a.exec();
}
