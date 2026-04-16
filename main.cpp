#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "Pcap.h"
#include "NicList.h"
#include "Capture.h"
#include "Open_List.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    qmlRegisterType<NicList>("Mini_Wireshark", 1, 0, "NicList");
    qmlRegisterType<Capture>("Mini_Wireshark", 1, 0, "Capture");
    qmlRegisterType<Pcap>("Mini_Wireshark", 1, 0, "Pcap");
    qmlRegisterType<Open_List>("Mini_Wireshark", 1, 0, "Open_List");

    qRegisterMetaType<Pcap*>("Pcap*");

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("Mini_Wireshark", "Main");
    return app.exec();
}
