#include "csaccount.h"
#include <QDebug>

CSAccount::CSAccount(QObject *parent) :
    QObject(parent)
{
}

CSAccount::CSAccount(QString email, QString login, QString pwd, QObject *parent) : QObject(parent) {
    if ( !email.isNull() && !email.isEmpty() ) {
        m_email = email;
    }
    if ( !login.isNull() && !login.isEmpty() ) {
        m_login = login;
    }
    if ( !pwd.isNull() && !pwd.isEmpty() ) {
        m_pwd = pwd;
    }
}

void CSAccount::setEMail(QString email) {
    if ( !email.isNull() && !email.isEmpty() ) {
        m_email = email;
    }
}

void CSAccount::setLogin(QString login) {
    if ( !login.isNull() && !login.isEmpty() ) {
        m_login = login;
    }
}

void CSAccount::setPassword(QString pwd) {
    if ( !pwd.isNull() && !pwd.isEmpty() ) {
        m_pwd = pwd;
    }
}

void CSAccount::addChannel(CSChannel *channel) {
    if ( channel && !m_channels.contains(channel) ) {
        m_channels.append(channel);
    }
}

bool CSAccount::checkChannel(CSChannel *channel) {
    if ( channel ) {
        return m_channels.contains(channel);
    }
    return false;
}

bool CSAccount::checkPassword(QString pwd) {
    return (pwd==m_pwd);
}

