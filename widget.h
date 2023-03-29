#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkInterface>
#include <QDebug>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    QTcpSocket *tcpSocket;
    QList<QHostAddress>IPlist;
    void getLocalHostIP();
private slots:
    void toConnect();
    void toDisConnect();
    void connected();
    void disconnected();
    void clearTextBrowser();
    void receiveMessages();
    void sendMessages();
    void sendMessagesHex();
    void socketStateChange(QAbstractSocket::SocketState);

    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_9_clicked();
};
#endif // WIDGET_H
