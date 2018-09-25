#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QDialog>
#include <QMainWindow>
#include <QtNetwork>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QByteArray>
#include <QAbstractSocket>

class QFile;
class QString;
class QTcpServer;
class QTcpSocket;


namespace Ui {
class frmMain;
}

class frmMain : public QDialog
{
    Q_OBJECT

public:
    explicit frmMain(QWidget *parent = 0);
    ~frmMain();
    void init();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);

private slots:
    void on_btnMenu_Close_clicked();

    void on_btnMenu_Max_clicked();

    void on_btnMenu_Min_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_Listen_clicked();

    void on_pushButton_Send_clicked();

    void server_New_Connect();

    void socket_Read_Data();

    void socket_Disconnected();


private:
    Ui::frmMain *ui;

    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;

    void InitStyle();
    QTcpServer* server;
    QTcpSocket* socket;
    QTimer *timer;
    QByteArray mChat;
    QTcpSocket *receive;
    qint64 gotBytes, fileBytes, nameSize;
    QFile *file;
    QString fileName;
};

#endif // FRMMAIN_H
