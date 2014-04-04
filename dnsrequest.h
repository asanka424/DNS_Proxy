#ifndef DNSREQUEST_H
#define DNSREQUEST_H

#include <QObject>
#include <QUdpSocket>
#include <QByteArray>
#include <QTimer>

class DNSRequest : public QObject
{
    Q_OBJECT
public:
    explicit DNSRequest(QByteArray request,QHostAddress sender, QHostAddress dnsHost,quint16 port,int _index,
                        QObject *parent = 0);
    ~DNSRequest();
    void setRequestIndex(int index){m_requestIndex = index;}
    void initRequest();
    QByteArray reply(){return m_reply;}
    QHostAddress originAddress(){return m_senderAddress;}
    quint16 originPort(){return m_senderPort;}

signals:
    void sig_dnsReplyCame(int index);

    void sig_timedOut(int index);

public slots:
    void slot_dnsReplyCame();

    void slot_dnsTimedOut();
private:
    QByteArray m_request;
    QByteArray m_reply;
    QHostAddress m_senderAddress,m_dnsHost;
    quint16 m_senderPort;
    int m_requestIndex;
    QTimer *mp_timer;
    QByteArray m_response;
    QUdpSocket *mp_localSocket,*mp_foreignSocket;

};

#endif // DNSREQUEST_H
