#include <QCoreApplication>
#include <QThread>
#include "logger.h"
#include "dnslistner.h"
#include <QHostAddress>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qRegisterMetaType<QHostAddress>("QHostAddress");
    QThread *p_listnerThread = new QThread();
    QThread *p_loggerThread = new QThread();

    Logger *p_logger = new Logger(QHostAddress("192.168.0.24"));
    DNSListner *p_listner = new DNSListner(QHostAddress::Any);

    QObject::connect(p_listner,SIGNAL(sig_newRequestReceived(QByteArray,QHostAddress,quint16)),
                     p_logger,SLOT(slot_newRequest(QByteArray,QHostAddress,quint16)),Qt::QueuedConnection);
    QObject::connect(p_logger,SIGNAL(sig_newReplyReceived(QByteArray,QHostAddress,quint16)),
                     p_listner,SLOT(slot_newReplyReceived(QByteArray,QHostAddress,quint16)),Qt::QueuedConnection);
    QObject::connect(p_listnerThread,SIGNAL(started()),p_listner,SLOT(slot_init()),Qt::QueuedConnection);
    p_logger->moveToThread(p_loggerThread);
    p_listner->moveToThread(p_listnerThread);
    p_loggerThread->start();
    p_listnerThread->start();

    return a.exec();
}
