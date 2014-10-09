#include "csnetmanager.h"

#include <QDebug>

csNetManager::csNetManager(QObject *parent) :
    QObject(parent)
{
    m_server = new QWebSocketServer("ChatServerListener",QWebSocketServer::NonSecureMode,this);
    if ( m_server->listen(QHostAddress::Any,8082) ) {
        qDebug() << "listening";
        connect(m_server,SIGNAL(newConnection()),this,SLOT(onNewConnection()));
    } else {
        qDebug() << "NOT listening";
    }
}

void csNetManager::onNewConnection() {
    qDebug() << "onNewConnection";
    QWebSocket * newClient = m_server->nextPendingConnection();
    if ( newClient!=Q_NULLPTR ) {
        m_clients.append(newClient);
        connect(newClient,SIGNAL(disconnected()),this,SLOT(socketDisconnected()));
        connect(newClient,SIGNAL(textMessageReceived(QString)),this,SLOT(processTextMessage(QString)));
        emit onNewClient(newClient);
    }
}

void csNetManager::sendTextToClient(QWebSocket *client, QString text) {
    qDebug() << "sendTextToClient";
    client->sendTextMessage(text);
}

void csNetManager::processTextMessage(QString msg) {
    qDebug() << "processTextMessage '" << msg << "' ";
    QWebSocket *curClient = qobject_cast<QWebSocket *>(sender());
//    foreach ( QWebSocket *client, m_clients ) {
//        if ( client!=curClient) {
//            client->sendTextMessage(msg);
//        }
//    }
    emit onNewMessage(msg,curClient);
}

void csNetManager::socketDisconnected() {
    qDebug() << "socketDisconnected";
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if ( pClient ) {
        emit onClientDisconnect(pClient);
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}

