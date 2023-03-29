#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("客户端");
    tcpSocket=new QTcpSocket(this);
    getLocalHostIP();
    connect(tcpSocket,SIGNAL(connected()),this,SLOT(connected()));
    connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(disconnected()));
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(receiveMessages()));
    connect(tcpSocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this,SLOT(socketStateChange(QAbstractSocket::SocketState)));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::getLocalHostIP()
{
    QList<QNetworkInterface> list
            = QNetworkInterface::allInterfaces();

    /* 遍历 list */
    foreach (QNetworkInterface interface, list) {

        /* QNetworkAddressEntry 类存储 IP 地址子网掩码和广播地址 */
        QList<QNetworkAddressEntry> entryList
                = interface.addressEntries();

        /* 遍历 entryList */
        foreach (QNetworkAddressEntry entry, entryList) {
            /* 过滤 IPv6 地址，只留下 IPv4 */
            if (entry.ip().protocol() ==
                    QAbstractSocket::IPv4Protocol) {
                ui->comboBox_2->addItem(entry.ip().toString());
                /* 添加到 IP 列表中 */
                IPlist<<entry.ip();
            }
        }
    }
    ui->comboBox_2->addItem("192.168.173.223");
}

void Widget::toConnect()
{
    if(tcpSocket->state()!=tcpSocket->ConnectedState){
        tcpSocket->connectToHost(ui->comboBox_2->currentText()/*IPlist[ui->comboBox_2->currentIndex()]*/,
                quint16(ui->spinBox_2->value()));
    }

}

void Widget::toDisConnect()
{
    tcpSocket->disconnectFromHost();
    tcpSocket->close();

}

void Widget::connected()
{
    ui->textBrowser->append("已经连上服务端");
    ui->pushButton_5->setEnabled(false);
    ui->pushButton_6->setEnabled(true);
    ui->comboBox_2->setEnabled(false);
    ui->spinBox_2->setEnabled(false);

}

void Widget::disconnected()
{
    ui->textBrowser->append("已经断开服务端");
    ui->pushButton_5->setEnabled(true);
    ui->pushButton_6->setEnabled(false);
    ui->comboBox_2->setEnabled(true);
    ui->spinBox_2->setEnabled(true);
}

void Widget::clearTextBrowser()
{
    ui->textBrowser->clear();
}

void Widget::receiveMessages()
{
    QString message=tcpSocket->readAll();
    ui->textBrowser->append("服务端："+message);

}

void Widget::sendMessages()
{

    if(nullptr==tcpSocket)
        return;
    if(tcpSocket->state()==tcpSocket->ConnectedState){
        ui->textBrowser->append("客户端："+ui->lineEdit_2->text());
        tcpSocket->write(ui->lineEdit_2->text().toUtf8().data());

    }
}

void Widget::sendMessagesHex()
{
    if(nullptr==tcpSocket)
        return;
    if(tcpSocket->state()==tcpSocket->ConnectedState){
        ui->textBrowser->append("客户端："+ui->lineEdit_3->text());

        tcpSocket->write(QByteArray::fromHex(ui->lineEdit_3->text().toLatin1()));
    }
}

void Widget::socketStateChange(QAbstractSocket::SocketState state)
{
    switch (state) {
    case QAbstractSocket::UnconnectedState:
        ui->textBrowser->append("socket状态：UnconnectedState");
        break;
    case QAbstractSocket::ConnectedState:
        ui->textBrowser->append("socket状态：ConnectedState");
        break;
    default:
        break;

    }
}


void Widget::on_pushButton_5_clicked()
{
    toConnect();
}

void Widget::on_pushButton_6_clicked()
{
    toDisConnect();
}

void Widget::on_pushButton_7_clicked()
{
    clearTextBrowser();
}

void Widget::on_pushButton_8_clicked()
{
    sendMessages();
}

void Widget::on_pushButton_9_clicked()
{
    sendMessagesHex();
}
