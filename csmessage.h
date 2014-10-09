#ifndef CSMESSAGE_H
#define CSMESSAGE_H

#include <QObject>
#include <QDateTime>

class QString;
class CSAccount;

class CSMessage : public QObject
{
    Q_OBJECT
public:
    explicit CSMessage(QObject *parent = 0);
    explicit CSMessage(QDateTime dt, QString msg, CSAccount* accInitiator, QObject *parent = 0);
    CSMessage(const CSMessage & obj);

    virtual ~CSMessage();

    CSMessage & operator=(const CSMessage & obj);
    bool operator==(const CSMessage & obj) const;
    inline bool operator!=(const CSMessage & obj) const{
        return !(*this == obj);
    }

    QDateTime getDateTime(void);
    QString getMessage(void);
    CSAccount* getAccInitiator(void);
    void setDateTime(QDateTime dt);
    void setMessage(QString msg);
    void setInitiator(CSAccount* accInitiator) { m_accInitiator=accInitiator; }
    QString toString(void);

signals:

public slots:

private:
    QDateTime m_datetime;
    QString m_message;
    CSAccount* m_accInitiator;

};

#endif // CSMESSAGE_H
