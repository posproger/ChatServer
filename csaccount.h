#ifndef CSACCOUNT_H
#define CSACCOUNT_H

#include <QObject>
#include <QList>
#include "cschannel.h"

class QString;

class CSAccount : public QObject
{
    Q_OBJECT
public:
    explicit CSAccount(QObject *parent = 0);
    explicit CSAccount(QString email, QString login, QString pwd, QObject *parent = 0);

    QString getEMail(void) { return m_email; }
    QString getLogin(void) { return m_login; }
    void setEMail(QString email);
    void setLogin(QString login);
    void setPassword(QString pwd);
    void addChannel(CSChannel* channel);
    bool checkChannel(CSChannel* channel);
    QList<CSChannel*> getAllChannels(void) { return m_channels; }
    bool checkPassword(QString pwd);

signals:

public slots:

private:
    QString m_email;
    QList<CSChannel*> m_channels;
    QString m_login;
    QString m_pwd;

};

#endif // CSACCOUNT_H
