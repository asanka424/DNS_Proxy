#ifndef MASTER_H
#define MASTER_H

#include <QObject>
#include <QList>
#include "dnsrequest.h"
#include <QMutexLocker>
#include "dnslistner.h"
#include <QDateTime>
#include <QFile>
#include <QThread>
#include <QDebug>
#include <QMap>

class Master : public QObject
{
    Q_OBJECT
public:
    explicit Master(QHostAddress dnsHost,int maxRequests = 10,QObject *parent = 0);
    ~Master();
    void init();
private:
    void logMessage(QString msg);


signals:
    void sig_newReplyReceived(QByteArray reply,QHostAddress receiver, quint16 port);
public slots:
    void slot_newRequest(QByteArray request, QHostAddress sender, quint16 port);
    //void slot_dnsRequestFinished(int index);
    void slot_timedOut(int index);
    void slot_dnsReplyCame(int index);
private:
    QList<DNSRequest*> m_requestList;
    int m_maxRequests;
    QHostAddress m_dnsHost;
    QMutex m_mutex;
    DNSListner *mp_listner;
    QString m_logFileName;
    QThread *mp_listnerThread;
    QMap<int,char> m_requestMap;

};

#endif // MASTER_H
