#include "cschannel.h"
#include <QString>

CSChannel::CSChannel(QObject *parent) :
    QObject(parent)
{
}

CSChannel::CSChannel(QString name, QObject *parent) : QObject(parent) {
    if ( !name.isEmpty() ) m_name = name;
}

CSChannel::CSChannel(const CSChannel & obj) : QObject(0) {
    m_name = obj.m_name;
    m_msgs = obj.m_msgs;
}

CSChannel::~CSChannel() {}

CSChannel & CSChannel::operator=(const CSChannel & obj) {
    m_name = obj.m_name;
    m_msgs = obj.m_msgs;
    return *this;
}

bool CSChannel::operator==(const CSChannel & obj) const {
    return (m_name==obj.m_name && m_msgs==obj.m_msgs);
}

void CSChannel::setName(QString name) {
    if ( !name.isEmpty() ) m_name = name;
}

QString CSChannel::getName(void) {
    return m_name;
}

void CSChannel::addMessage(CSMessage csmsg) {
    m_msgs.append(csmsg);
    emit onNewChannelMessage(csmsg);
}

void CSChannel::clearMessages() {
    m_msgs.clear();
}

