#include <QCoreApplication>
#include <QThread>
#include "master.h"
#include "dnslistner.h"
#include <QHostAddress>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qRegisterMetaType<QHostAddress>("QHostAddress");


    Master *p_master = new Master(QHostAddress("8.8.8.8"));
    p_master->init();

    return a.exec();
}
