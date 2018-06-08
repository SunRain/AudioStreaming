#ifndef SSLCLIENT_H
#define SSLCLIENT_H

#include <QtCore>
#include <QtNetwork>
#include "common.h"

class SslClient : public QObject
{
    Q_OBJECT
public:
    explicit SslClient(QObject *parent = nullptr);
    ~SslClient();

signals:
    void connectedToServer(QByteArray);
    void pending(quint32, QByteArray);
    void disconnected();
    void connectionInfo(quint32, QByteArray, QByteArray);
    void remotePort(quint16);
    void error(QString);

public slots:
    void connectToHost(const QString &host, quint16 port);
    void stop();
    int write(const QByteArray &data);

private slots:
    void sslErrors(QList<QSslError> errors);
    void timeout();
    void encrypted();
    void disconnectedPrivate();
    void errorPrivate(QAbstractSocket::SocketError e);
    void readyReadPrivate();
    void processInput(const QByteArray &peer_data);

private:
    QSslSocket *m_socket;
    QByteArray m_buffer;
    qint32 m_size;
    QTimer *m_timer;
};

#endif // SSLCLIENT_H
