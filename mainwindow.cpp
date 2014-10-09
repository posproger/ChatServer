#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&netManager,SIGNAL(onNewMessage(QString,QWebSocket*)),this,SLOT(newMsgCome(QString)));
    connect(&netManager,SIGNAL(onNewClient(QWebSocket*)),this,SLOT(newClientCome(QWebSocket*)));

    connect(&netManager,SIGNAL(onNewClient(QWebSocket*)),&m_clientManager,SLOT(onNewClient(QWebSocket*)));
    connect(&netManager,SIGNAL(onClientDisconnect(QWebSocket*)),&m_clientManager,SLOT(onClientDisconnect(QWebSocket*)));
    connect(&netManager,SIGNAL(onNewMessage(QString, QWebSocket*)),&m_clientManager,SLOT(onNewMessage(QString, QWebSocket*)));
    connect(&m_clientManager,SIGNAL(sendTextToClient(QWebSocket*, QString)),&netManager,SLOT(sendTextToClient(QWebSocket*, QString)));
    m_clientManager.init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newMsgCome(QString msg) {
    ui->tbMainText->append(msg);
}

QString MainWindow::getHistory(void) {
    return QString() + ui->tbMainText->toPlainText();
}

void MainWindow::newClientCome(QWebSocket *client) {
    qDebug() << "newClientCome" << client;
    //netManager.sendTextToClient(client,getHistory());
}
