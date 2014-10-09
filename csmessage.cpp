#include "csmessage.h"

#include <QDateTime>
#include <QString>
#include "csaccount.h"

CSMessage::CSMessage(QObject *parent) :
    QObject(parent)
{
}

CSMessage::CSMessage(QDateTime dt, QString msg, CSAccount *accInitiator, QObject *parent) : QObject(parent) {
    if ( dt.isValid() ) m_datetime = dt;
    if ( !msg.isEmpty() ) m_message = msg;
    m_accInitiator = accInitiator;
}

CSMessage::CSMessage(const CSMessage & obj) : QObject(0) {
    m_datetime = obj.m_datetime;
    m_message = obj.m_message;
    m_accInitiator = obj.m_accInitiator;
}

CSMessage::~CSMessage(){}


CSMessage & CSMessage::operator=(const CSMessage & obj) {
    m_datetime = obj.m_datetime;
    m_message = obj.m_message;
    m_accInitiator = obj.m_accInitiator;
    return *this;
}

bool CSMessage::operator==(const CSMessage & obj) const {
    return m_datetime==obj.m_datetime && m_message==obj.m_message && m_accInitiator==obj.m_accInitiator;
}


QDateTime CSMessage::getDateTime(void) {
    return m_datetime;
}

QString CSMessage::getMessage(void) {
    return m_message;
}

void CSMessage::setDateTime(QDateTime dt) {
    if ( dt.isValid() ) m_datetime = dt;
}

void CSMessage::setMessage(QString msg) {
    if ( !msg.isEmpty() ) m_message = msg;
}

CSAccount* CSMessage::getAccInitiator(void) {
    return m_accInitiator;
}

QString CSMessage::toString(void) {
    return getDateTime().toString("HH:mm:ss")+" "+getAccInitiator()->getLogin()+": "+getMessage();
}

