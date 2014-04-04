#include "logger.h"

Logger::Logger(QHostAddress _dnsHost, QObject *parent) :
    m_dnsHost(_dnsHost),QObject(parent)
{
}
Logger::~Logger()
{
    for (int i=0; i<m_requestList.size(); i++)
    {
        m_requestList.at(i)->deleteLater();
    }
}

void Logger::slot_newRequest(QByteArray request, QHostAddress sender, quint16 port)
{
    QMutexLocker ml(&m_mutex);
    int index = m_requestList.size();
    DNSRequest *tmpReq = new DNSRequest(request,sender,m_dnsHost,port,index);
    connect(tmpReq,SIGNAL(sig_dnsReplyCame(int)),
            this,SLOT(slot_dnsReplyCame(int)),Qt::QueuedConnection);
    connect(tmpReq,SIGNAL(sig_timedOut(int)),this,SLOT(slot_timedOut(int)),Qt::QueuedConnection);

    m_requestList.push_back(tmpReq);
    tmpReq->initRequest();
}
void Logger::slot_dnsReplyCame(int index)
{
    emit sig_newReplyReceived(m_requestList.at(index)->reply(),
                              m_requestList.at(index)->originAddress(),
                              m_requestList.at(index)->originPort());
    QMutexLocker ml(&m_mutex);
    delete m_requestList.at(index);
    m_requestList.removeAt(index);
}
void Logger::slot_dnsRequestFinished(int index)
{
    QMutexLocker ml(&m_mutex);
    delete m_requestList.at(index);
    m_requestList.removeAt(index);
}

void Logger::slot_timedOut(int index)
{
    QMutexLocker ml(&m_mutex);
    delete m_requestList.at(index);
    m_requestList.removeAt(index);
}

