#include "master.h"

Master::Master(QHostAddress _dnsHost, int maxRequests, QObject *parent) :
    m_dnsHost(_dnsHost),mp_listner(new DNSListner(QHostAddress::Any)),
    mp_listnerThread(new QThread),m_maxRequests(maxRequests),QObject(parent)
{
}
Master::~Master()
{
    for (int i=0; i<m_requestMap.size(); i++)
    {
        if (m_requestMap[i] == 'A')
            m_requestList.at(i)->deleteLater();
    }
    mp_listner->deleteLater();
    mp_listnerThread->deleteLater();
}

void Master::slot_newRequest(QByteArray request, QHostAddress sender, quint16 port)
{
    QMutexLocker ml(&m_mutex);


    int index = -1;
    for (int i=0; i<m_maxRequests; i++)
    {
        if (m_requestMap[i] == 'I')
        {
            index = i;
            m_requestMap[i] = 'A';
            break;
        }
    }
    if (index == -1)
    {
        logMessage(QString("Request dropped due to resource constrains") + QString::number(index));
        return;
    }
    DNSRequest *tmpReq = new DNSRequest(request,sender,m_dnsHost,port,index);
    connect(tmpReq,SIGNAL(sig_dnsReplyCame(int)),
            this,SLOT(slot_dnsReplyCame(int)),Qt::QueuedConnection);
    connect(tmpReq,SIGNAL(sig_timedOut(int)),this,SLOT(slot_timedOut(int)),Qt::QueuedConnection);

    m_requestList[index] = tmpReq;
    tmpReq->initRequest();
    logMessage(QString("Request came and request object created with index ") + QString::number(index));
}
void Master::slot_dnsReplyCame(int index)
{
    emit sig_newReplyReceived(m_requestList.at(index)->reply(),
                              m_requestList.at(index)->originAddress(),
                              m_requestList.at(index)->originPort());
    QMutexLocker ml(&m_mutex);
    delete m_requestList.at(index);
    m_requestMap[index] = 'I';
    logMessage(QString("Reply came for request index ") + QString::number(index));
}
/*void Master::slot_dnsRequestFinished(int index)
{
    QMutexLocker ml(&m_mutex);
    delete m_requestList.at(index);
    m_requestMap[index] = 'I';
}*/

void Master::slot_timedOut(int index)
{
    QMutexLocker ml(&m_mutex);
    delete m_requestList.at(index);
    m_requestMap[index] = 'I';
    logMessage(QString("Request timeout for request index ") + QString::number(index));
}

void Master::logMessage(QString msg)
{
    qDebug() << msg;
    if (!m_logFileName.isEmpty())
    {
        QString timeStamp = QDateTime::currentDateTime().toString(QString("yyyy-MM-dd | hh:mm:ss"));
        QFile file(m_logFileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Append))
        {
            QTextStream out(&file);
            out << timeStamp << " | " << msg << "\n";
            file.close();
        }
    }
}
void Master::init()
{
    m_requestList.reserve(m_maxRequests);
    DNSRequest *dump;
    for (int i=0; i<m_maxRequests; i++)
    {
        m_requestMap[i] = 'I';
        m_requestList.append(dump);
    }
    connect(mp_listner,SIGNAL(sig_newRequestReceived(QByteArray,QHostAddress,quint16)),
                     this,SLOT(slot_newRequest(QByteArray,QHostAddress,quint16)),Qt::QueuedConnection);
    connect(this,SIGNAL(sig_newReplyReceived(QByteArray,QHostAddress,quint16)),
                     mp_listner,SLOT(slot_newReplyReceived(QByteArray,QHostAddress,quint16)),Qt::QueuedConnection);
    connect(mp_listnerThread,SIGNAL(started()),mp_listner,SLOT(slot_init()),Qt::QueuedConnection);
    mp_listner->moveToThread(mp_listnerThread);
    mp_listnerThread->start();

}


