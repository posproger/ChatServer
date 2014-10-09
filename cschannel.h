#ifndef CSCHANNEL_H
#define CSCHANNEL_H

#include <QObject>
#include <QList>
#include "csmessage.h"

class QString;

class CSChannel : public QObject
{
    Q_OBJECT
public:
    explicit CSChannel(QObject *parent = 0);
    explicit CSChannel(QString name, QObject *parent = 0);
    explicit CSChannel(const CSChannel & obj);

    virtual ~CSChannel();

    CSChannel & operator=(const CSChannel & obj);
    bool operator==(const CSChannel & obj) const;
    inline bool operator!=(const CSChannel & obj) const{
        return !(*this == obj);
    }

    void setName(QString name);
    QString getName(void);
    QList<CSMessage> getAllMessages(void) { return m_msgs; }

signals:
    void onNewChannelMessage(CSMessage &csmsg);

public slots:
    void addMessage(CSMessage csmsg);
    void clearMessages();

private:
    //unsigned long m_id;
    QString m_name;
    QList<CSMessage> m_msgs;

};

#endif // CSCHANNEL_H
