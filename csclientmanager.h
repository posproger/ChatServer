#ifndef CSCLIENTMANAGER_H
#define CSCLIENTMANAGER_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QtWebSockets/QtWebSockets>
#include "csaccount.h"
#include "cschannel.h"
#include "csmessage.h"

class CSClientManager : public QObject
{
    Q_OBJECT
public:
    explicit CSClientManager(QObject *parent = 0);

    void init(void); /// !!!!! Код для тестов

signals:
    void sendTextToClient(QWebSocket *client, QString text);

public slots:
    void onNewClient(QWebSocket *client);
    void onClientDisconnect(QWebSocket *client);
    void onNewMessage(QString msg, QWebSocket *client);
    void onNewChannelMessage(CSMessage &csmsg);

private:
    QMap<int,CSChannel*> m_channels;
    QMap<QWebSocket*,CSAccount*> m_accounts;
    QMap<int,QList<QWebSocket*> > m_channelClients;

    CSMessage parseStringMessage(QString msg, QWebSocket *client);

/// Код для тестов
    QList<CSAccount*> m_allAccounts; /// !!!!!
    CSAccount * findAccountByLogin(QString login);
/// eof Код для тестов

};

#endif // CSCLIENTMANAGER_H
