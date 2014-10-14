#include "csclientmanager.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

CSClientManager::CSClientManager(QObject *parent) :
    QObject(parent)
{
}

void CSClientManager::init(void) {
    /// Временный код для тестов !!!!!
    CSChannel *ch1 = new CSChannel("Public");
    CSChannel *ch2 = new CSChannel("Private1");
    CSChannel *ch3 = new CSChannel("Private2");

    //m_channels[0] - не использовать!
    m_channels[1] = ch1;
    connect(ch1,SIGNAL(onNewChannelMessage(CSMessage&)),this,SLOT(onNewChannelMessage(CSMessage&)));
    m_channels[2] = ch2;
    connect(ch2,SIGNAL(onNewChannelMessage(CSMessage&)),this,SLOT(onNewChannelMessage(CSMessage&)));
    m_channels[3] = ch3;
    connect(ch3,SIGNAL(onNewChannelMessage(CSMessage&)),this,SLOT(onNewChannelMessage(CSMessage&)));

    CSAccount *acc1 = new CSAccount("dima@mail.mu", "Dima", "850a3a6f4fda0c51073cf7644807469b6ce78ff21314a3ad154c0a077b0476a8");
    CSAccount *acc2 = new CSAccount("defoer@mail.mu", "Def", "fcc0fc8f7736ba6f8fafae6842fdae3b2cb7339870291e5cc1f9dbe5a9dfc850");
    CSAccount *acc3 = new CSAccount("max@mail.mu", "Max", "a1a5936d3b0f8a69fd62c91ed9990d3bd414c5e78c603e2837c65c9f46a93eb8");
    CSAccount *acc4 = new CSAccount("ura@mail.mu", "Ura", "3edca13eeb5104c233fcc46bb6338a69ecdf838c45dbf1357d1e481c3c8d78da");
    CSAccount *acc5 = new CSAccount("", "guest", "");
    CSAccount *acc6 = new CSAccount("admin@mail.mu", "admin", "8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918");
    CSAccount *acc7 = new CSAccount("nemo@mail.mu", "nemo", "0075870f97c892ba9a7243718e397d34ec7966d4fd9782974c0d2d6406797139");

    m_allAccounts.append(acc1);
    m_allAccounts.append(acc2);
    m_allAccounts.append(acc3);
    m_allAccounts.append(acc4);
    m_allAccounts.append(acc5);
    m_allAccounts.append(acc6);
    m_allAccounts.append(acc7);

    acc1->addChannel(ch1);
    acc1->addChannel(ch2);
    acc1->addChannel(ch3);
    acc2->addChannel(ch1);
    acc2->addChannel(ch2);
    acc2->addChannel(ch3);
    acc3->addChannel(ch1);
    acc3->addChannel(ch2);
    acc3->addChannel(ch3);
    acc4->addChannel(ch1);
    acc4->addChannel(ch2);
    acc5->addChannel(ch1);
    acc6->addChannel(ch1);
    acc6->addChannel(ch2);
    acc6->addChannel(ch3);
    acc7->addChannel(ch1);
}

void CSClientManager::onNewClient(QWebSocket *client) {
    if ( !m_accounts.contains(client) ) {
        m_accounts.insert(client,0);
    }
}

void CSClientManager::onClientDisconnect(QWebSocket *client) {
    if ( m_accounts.contains(client) ) {
        m_accounts.remove(client);
    }
    foreach (QList<QWebSocket*>clients, m_channelClients) {
        if ( clients.contains(client) ) {
            clients.removeAll(client);
        }
    }
}

void CSClientManager::onNewMessage(QString msg, QWebSocket *client) {
    qDebug() << "CSClientManager::onNewMessage";
    if ( !msg.isEmpty() && client ) {
        CSMessage csmsg = parseStringMessage(msg, client);
    }
}

CSMessage CSClientManager::parseStringMessage(QString msg, QWebSocket *client) {
    qDebug() << "CSClientManager::parseStringMessage";
    CSMessage csmsg;
    if ( !msg.isEmpty() ) {
        QJsonDocument jdoc(QJsonDocument::fromJson(msg.toUtf8()));
        if ( !jdoc.isEmpty() ) {
            QJsonObject jobj = jdoc.object();
            QString cmd = jobj["c"].toString();
            if ( cmd==QString("REG") ) {
                qDebug() << "REG cmd";
                QString login = jobj["l"].toString();
                QString mail = jobj["m"].toString();
                QString pwd = jobj["p"].toString();
                CSAccount *acc = findAccountByLogin(login);
                QJsonObject joAnswer;
                joAnswer["c"]=QString("REG");
                if ( acc && acc->checkPassword(pwd) && acc->getEMail()==mail ) {
                    if ( m_accounts[client]==0 ) {
                        m_accounts[client] = acc;
                        joAnswer["res"]=QString("OK");
                        QList<CSChannel*> chs = acc->getAllChannels();
                        foreach(CSChannel* ch,chs) {
                            int index = m_channels.key(ch,0);
                            if ( index!=0 ) {
                                m_channelClients[index].append(client);
                            }
                        }
                    } else {
                        qDebug() << "REG ERROR:" << "l =" << login << "Socket busy!";
                        /// ????? Что делать если акк на сокете уже есть ??? Старого вышибать?
                        joAnswer["res"]=QString("ER");
                    }
                } else {
                    if ( acc ) {
                        qDebug() << "REG ERROR:" << "l =" << login << "m =" << mail << "p =" << pwd << "acc =" << acc << acc->checkPassword(pwd) << "m1 =" << acc->getEMail();
                    } else {
                        qDebug() << "REG ERROR:" << "l =" << login << "m =" << mail << "p =" << pwd << "acc =" << acc;
                    }
                    joAnswer["res"]=QString("ER");
                }
                QJsonDocument jdAnswer(joAnswer);
                emit sendTextToClient(client, QString(jdAnswer.toJson()));
            } else if ( cmd==QString("GCH") ) {
                qDebug() << "GCH cmd";
                QJsonObject joAnswer;
                joAnswer["c"]=QString("GCH");
                CSAccount *acc = m_accounts[client];
                if ( acc ) {
                    joAnswer["res"]=QString("OK");
                    QList<CSChannel*> chs = acc->getAllChannels();
                    int counter = 0;
                    QJsonArray chArray;
                    foreach(CSChannel* ch,chs) {
                        int index = m_channels.key(ch,0);
                        if ( index!=0 ) {
                            QJsonObject chObject;
                            chObject["i"] = index;
                            chObject["n"] = ch->getName();
                            chArray.append(chObject);
                            ++counter;
                        }
                    }
                    joAnswer["n"]=counter;
                    if ( counter>0 ) joAnswer["chs"]=chArray;
                } else {
                    joAnswer["res"]=QString("ER");
                }
                QJsonDocument jdAnswer(joAnswer);
                emit sendTextToClient(client, QString(jdAnswer.toJson()));
            } else if ( cmd==QString("CHH") ) {
                qDebug() << "CHH cmd";
                int chid = jobj["ch"].toInt();
                CSAccount *acc = m_accounts[client];
                QJsonObject joAnswer;
                joAnswer["c"]=QString("CHH");
                joAnswer["res"]=QString("ER");
                if ( acc && chid>0 ) {
                    CSChannel* ch = m_channels[chid];
                    if ( ch ) {
                        joAnswer["res"]=QString("OK");
                        QJsonArray chArray;
                        foreach( CSMessage csmsg, ch->getAllMessages() ) {
                            chArray.append(csmsg.toString());
                        }
                        joAnswer["msgs"]=chArray;
                    }
                }
                QJsonDocument jdAnswer(joAnswer);
                emit sendTextToClient(client, QString(jdAnswer.toJson()));
            } else if ( cmd==QString("MSG") ) {
                qDebug() << "MSG cmd";
                QString sid = jobj["id"].toString();
                int chid = jobj["ch"].toInt();
                QString msg = jobj["msg"].toString();
                QJsonObject joAnswer;
                joAnswer["c"]=QString("MSG");
                joAnswer["id"]=sid;
                joAnswer["res"]=QString("ER");
                CSAccount *acc = m_accounts[client];
                if ( acc && chid>0 ) {
                    CSChannel* ch = m_channels[chid];
                    if ( ch ) {
                        CSMessage csmsg(QDateTime::currentDateTimeUtc(),msg,acc);
                        ch->addMessage(csmsg);
                        joAnswer["res"]=QString("OK");
                    }
                }
                QJsonDocument jdAnswer(joAnswer);
                emit sendTextToClient(client, QString(jdAnswer.toJson()));
//            } else if ( cmd==QString("") ) {
//                ;
            } else {
                qDebug() << "Unknown command" << cmd << " msg:" << msg;
            }
        }
    }
    return csmsg;
}

CSAccount * CSClientManager::findAccountByLogin(QString login) {
    qDebug() << "CSClientManager::findAccountByLogin  l = " << login;
    foreach ( CSAccount *acc, m_allAccounts ) {
        if ( acc->getLogin()==login ) {
            return acc;
        }
    }
    return 0;
}

void CSClientManager::onNewChannelMessage(CSMessage &csmsg) {
    qDebug() << "CSClientManager::onNewChannelMessage";
    CSChannel* ch = dynamic_cast<CSChannel*>(sender());
    if ( ch ) {
        int index = m_channels.key(ch,0);
        if ( index!=0 ) {
            QJsonObject joAnswer;
            joAnswer["c"]=QString("MSG");
            joAnswer["id"]=QString("0");
            joAnswer["res"]=QString("IN");
            joAnswer["ch"]=index;
            joAnswer["msg"]=csmsg.toString();
            QJsonDocument jdAnswer(joAnswer);
            foreach ( QWebSocket *client, m_channelClients[index] ) {
                if ( client && m_accounts[client] ) { //&& (!csmsg.getAccInitiator() || m_accounts[client]!=csmsg.getAccInitiator()) ) {
                    emit sendTextToClient(client, QString(jdAnswer.toJson()));
                }
            }
        }
    }
}

