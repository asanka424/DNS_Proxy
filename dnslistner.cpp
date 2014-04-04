#include "dnslistner.h"

DNSListner::DNSListner(QHostAddress _host, QObject *parent) :
    m_dnsHost(_host),m_initiated(false),QObject(parent)
{
}

void DNSListner::slot_init()
{
    mp_listnerSocket = new QUdpSocket();
    connect(mp_listnerSocket,SIGNAL(readyRead()),this,SLOT(slot_readRequest()),Qt::DirectConnection);

    mp_listnerSocket->bind(m_dnsHost,53);
}

void DNSListner::slot_readRequest()
{
    QHostAddress sender;
    quint16 port;
    QByteArray request;
   // QMutexLocker ml(&m_mutex);
    if (mp_listnerSocket->hasPendingDatagrams())
    {
        qint64 size = mp_listnerSocket->pendingDatagramSize();
        request.resize(size);
        mp_listnerSocket->readDatagram(request.data(),size,&sender,&port);

        emit sig_newRequestReceived(request,sender,port);
    }

   // ml.unlock();

}
void DNSListner::slot_newReplyReceived(QByteArray reply, QHostAddress receiver, quint16 port)
{
    mp_listnerSocket->writeDatagram(reply,receiver,port);
}
