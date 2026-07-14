#include "PlatformCompat.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "Pcap.h"
#include "NicList.h"
#include "Capture.h"
#include "Open_List.h"

int main(int argc, char *argv[])
{
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        return -1;
    }
#endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    qmlRegisterType<NicList>("Mini_Wireshark", 1, 0, "NicList");
    qmlRegisterType<Capture>("Mini_Wireshark", 1, 0, "Capture");
    qmlRegisterType<Pcap>("Mini_Wireshark", 1, 0, "Pcap");
    qmlRegisterType<Open_List>("Mini_Wireshark", 1, 0, "Open_List");

    qRegisterMetaType<Pcap*>("Pcap*");
    qRegisterMetaType<st_pkt>("st_pkt");
    qRegisterMetaType<dump_data>("dump_data");

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("Mini_Wireshark", "Main");
    int exitCode = app.exec();

#ifdef _WIN32
    WSACleanup();
#endif

    return exitCode;
}
