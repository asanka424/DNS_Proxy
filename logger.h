#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QList>
#include "dnsrequest.h"
#include <QMutexLocker>

class Logger : public QObject
{
    Q_OBJECT
public:
    explicit Logger(QHostAddress dnsHost,QObject *parent = 0);
    ~Logger();

signals:
    void sig_newReplyReceived(QByteArray reply,QHostAddress receiver, quint16 port);
public slots:
    void slot_newRequest(QByteArray request, QHostAddress sender, quint16 port);
    void slot_dnsRequestFinished(int index);
    void slot_timedOut(int index);
    void slot_dnsReplyCame(int index);
private:
    QList<DNSRequest*> m_requestList;
    int m_maxRequests;
    QHostAddress m_dnsHost;
    QMutex m_mutex;

};

#endif // LOGGER_H
