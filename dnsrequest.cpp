#include "dnsrequest.h"

DNSRequest::DNSRequest(QByteArray _request, QHostAddress _sender, QHostAddress _dnsHost, quint16 _port, int _index, QObject *parent) :
    m_request(_request),m_senderAddress(_sender),m_dnsHost(_dnsHost),m_senderPort(_port),m_requestIndex(_index),mp_timer(new QTimer),QObject(parent)
{
    mp_timer->setSingleShot(true);
    connect(mp_timer,SIGNAL(timeout()),this,SLOT(slot_dnsTimedOut()));
    
}

DNSRequest::~DNSRequest()
{
    if (mp_timer->isActive())
        mp_timer->stop();
    mp_timer->deleteLater();
    mp_foreignSocket->deleteLater();//make sure to call initRequest
}

void DNSRequest::initRequest()
{
    mp_foreignSocket = new QUdpSocket();
    connect(mp_foreignSocket,SIGNAL(readyRead()),this,SLOT(slot_dnsReplyCame()),Qt::QueuedConnection);
    mp_foreignSocket->writeDatagram(m_request,m_dnsHost,53);
    mp_timer->start(3000);
}
void DNSRequest::slot_dnsReplyCame()
{
    if (mp_foreignSocket->hasPendingDatagrams())
    {
        qint64 size = mp_foreignSocket->pendingDatagramSize();
        m_reply.resize(size);
        mp_foreignSocket->readDatagram(m_reply.data(),size);
        emit sig_dnsReplyCame(m_requestIndex);
    }

}


void DNSRequest::slot_dnsTimedOut()
{
    emit sig_timedOut(m_requestIndex);
}
