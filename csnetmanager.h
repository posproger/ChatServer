#ifndef CSNETMANAGER_H
#define CSNETMANAGER_H

#include <QObject>
#include <QtWebSockets/QtWebSockets>
#include <QList>

class csNetManager : public QObject
{
    Q_OBJECT
public:
    explicit csNetManager(QObject *parent = 0);

signals:
    void onNewMessage(QString msg, QWebSocket *client);
    void onNewClient(QWebSocket *client);
    void onClientDisconnect(QWebSocket *client);

public slots:
    void onNewConnection();
    void processTextMessage(QString message);
    void socketDisconnected();
    void sendTextToClient(QWebSocket *client, QString text);

private:
    QWebSocketServer *m_server;
    QList<QWebSocket*> m_clients;

};

#endif // CSNETMANAGER_H
