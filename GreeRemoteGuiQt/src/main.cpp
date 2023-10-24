#include "mainwindow.h"
#include <QApplication>
#include <QLoggingCategory>
#include <QObject>
#include <devicefinder.h>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    a.setAttribute(Qt::AA_EnableHighDpiScaling);

    qInfo() << "initializing";

    DeviceFinder dh;

    MainWindow w(dh);
    w.show();

    QObject::connect(&dh, &DeviceFinder::scanFinshed, [] { qInfo() << "scanning finished"; });
    QObject::connect(&w, &MainWindow::scanInitiated, &dh, &DeviceFinder::scan);
    QObject::connect(&dh, &DeviceFinder::scanFinshed, &w, &MainWindow::onScanFinished);
    QObject::connect(&dh, &DeviceFinder::deviceBound, [&w, &dh](const DeviceDescriptor& descriptor) { w.addDevice(dh.getDevice(descriptor)); });

    dh.scan();

    auto exitCode = a.exec();

    qInfo() << "finished with exit code" << exitCode;

    return exitCode;
}
