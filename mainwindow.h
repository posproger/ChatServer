#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "csnetmanager.h"
#include "csclientmanager.h"

class QString;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString getHistory(void);

public slots:
    void newMsgCome(QString msg);
    void newClientCome(QWebSocket *client);

private:
    Ui::MainWindow *ui;
    csNetManager netManager;
    CSClientManager m_clientManager;

};

#endif // MAINWINDOW_H

