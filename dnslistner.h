#ifndef DNSLISTNER_H
#define DNSLISTNER_H

#include <QObject>
#include <QUdpSocket>
#include <QByteArray>
#include <QMutexLocker>
#include <QCoreApplication>

class DNSListner : public QObject
{
    Q_OBJECT
public:
    explicit DNSListner(QHostAddress _host, QObject *parent = 0);


signals:
    void sig_newRequestReceived(QByteArray request, QHostAddress sender, quint16 port);

public slots:
    void slot_readRequest();
    void slot_init();
    void slot_newReplyReceived(QByteArray reply,QHostAddress receiver, quint16 port);
private:
    QHostAddress m_dnsHost;
    bool m_initiated;
    QUdpSocket *mp_listnerSocket;
    QMutex m_mutex;

};

#endif // DNSLISTNER_H
