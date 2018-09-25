#include "frmmain.h"
#include "ui_frmmain.h"
#include "iconhelper.h"
#include "myhelper.h"
#include "encryption.h"
#include "decryption.h"
#include "rc4.h"
#include "des.h"
#include "rsa.h"
#include "md5.h"
#include "dh_fun.h"
#include "fun_dh_simple.h"
#include <QObject>
#include <QString>
#include <QTextEdit>
#include <QHostAddress>
#include <QByteArray>
#include <QFileDialog>
#include "mainwindow.h"

using namespace std;

static BIGNUM M,C,d,n,e, p, q;
static char md5_char[100] = {0};
big_int big_p, bg, A, B, A_send, B_send, K1, K2;
static long int sim_p, sim_g,a,b,a_send,b_send,k1,k2;


const quint16 PORT = 3333;
const qint64 LOADBYTES = 4 * 1024; // 4 kilo-byte
const int DATA_STREAM_VERSION = QDataStream::Qt_4_8;


frmMain::frmMain(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmMain)
{
    ui->setupUi(this);

    myHelper::FormInCenter(this);
    this->InitStyle();
    
    socket = new QTcpSocket();

    //连接信号槽
    QObject::connect(socket, &QTcpSocket::readyRead, this, &frmMain::socket_Read_Data);
    QObject::connect(socket, &QTcpSocket::disconnected, this, &frmMain::socket_Disconnected);

    ui->pushButton_Send->setEnabled(false);
    ui->lineEdit_IP->setText("127.0.0.1");
    ui->lineEdit_Port->setText("8765");

    sendf = new QTcpSocket(this);
    fileBytes = sentBytes = restBytes = 0;
    loadBytes = LOADBYTES;
    file = Q_NULLPTR;
}

frmMain::~frmMain()
{
    delete ui;
}

void frmMain::InitStyle()
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    location = this->geometry();
    max = false;
    mousePressed = false;

    ui->lab_Title->installEventFilter(this);

    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf096), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Min, QChar(0xf068), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu, QChar(0xf0c9), 10);
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf015), 12);
}

bool frmMain::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void frmMain::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void frmMain::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void frmMain::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void frmMain::on_btnMenu_Close_clicked()
{
    qApp->exit();
}

void frmMain::on_btnMenu_Max_clicked()
{
    if (max) {
        this->setGeometry(location);
        IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf096), 10);
        ui->btnMenu_Max->setToolTip("最大化");
    } else {
        location = this->geometry();
        this->setGeometry(qApp->desktop()->availableGeometry());
        IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf079), 10);
        ui->btnMenu_Max->setToolTip("还原");
    }
    max = !max;
}

void frmMain::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}


//加密
void frmMain::on_pushButton_12_clicked()
{

    //Caesar 加密函数
    if(ui->tabWidget->currentIndex()==0 && ui->toolBox->currentIndex()==0 && ui->radioButton_1->isChecked()){
        char temp[500] = {0};
        int key = this -> ui -> lineEdit -> text().toInt();
        QString qstr = this -> ui -> plainTextEdit_1->toPlainText();
        string st = qstr.toStdString();
        string str = encryCaesar(key, st, temp);
        QString sresult = QString::fromStdString(str);
        this -> ui -> plainTextEdit_2 -> setPlainText(sresult);
    }

    //Keyword  加密函数
    if(ui->tabWidget->currentIndex()==0 && ui->toolBox->currentIndex()==0 && ui->radioButton_2->isChecked()){
        char temp[500] = {0};
        QString qkey = this -> ui -> lineEdit -> text();
        string key = qkey.toStdString();
        QString qstr = this -> ui -> plainTextEdit_1->toPlainText();
        string st = qstr.toStdString();
        string str = encryKeyword(key, st, temp);
        QString sresult = QString::fromStdString(str);
        this -> ui -> plainTextEdit_2 -> setPlainText(sresult);
    }

    //Vigenere  加密函数
    if(ui->tabWidget->currentIndex()==0 && ui->toolBox->currentIndex()==1 && ui->radioButton_5->isChecked()){
        char temp[500] = {0};
        QString qkey = this -> ui -> lineEdit -> text();
        string key = qkey.toStdString();
        QString qstr = this -> ui -> plainTextEdit_1->toPlainText();
        string st = qstr.toStdString();
        string str = encryVigenere(key, st, temp);
        QString sresult = QString::fromStdString(str);
        this -> ui -> plainTextEdit_2 -> setPlainText(sresult);
    }

    //Autokey ciphertext 加密函数
    if(ui->tabWidget->currentIndex()==0 && ui->toolBox->currentIndex()==1 && ui->radioButton_6->isChecked()){
        char temp[500] = {0};
        QString qkey = this -> ui -> lineEdit -> text();
        string key = qkey.toStdString();
        QString qstr = this -> ui -> plainTextEdit_1->toPlainText();
        string st = qstr.toStdString();
        string str = encryAutokey(key, st, temp);
        QString sresult = QString::fromStdString(str);
        this -> ui -> plainTextEdit_2 -> setPlainText(sresult);
    }

    //Autokey plaintext 加密函数
    if(ui->tabWidget->currentIndex()==0 && ui->toolBox->currentIndex()==1 && ui->radioButton_7->isChecked()){
        char temp[500] = {0};
        QString qkey = this -> ui -> lineEdit -> text();
        string key = qkey.toStdString();
        QString qstr = this -> ui -> plainTextEdit_1->toPlainText();
        string st = qstr.toStdString();
        string str = encryAutokey2(key, st, temp);
        QString sresult = QString::fromStdString(str);
        this -> ui -> plainTextEdit_2 -> setPlainText(sresult);
    }

    //Playfair 加密函数
    if(ui->tabWidget->currentIndex()==0 && ui->toolBox->currentIndex()==2 && ui->radioButton_8->isChecked()){
        char temp[500] = {0};
        QString qkey = this -> ui -> lineEdit -> text();
        string key = qkey.toStdString();
        QString qstr = this -> ui -> plainTextEdit_1->toPlainText();
        string st = qstr.toStdString();
        string str = encryPlayfair(key, st, temp);
        QString sresult = QString::fromStdString(str);
        this -> ui -> plainTextEdit_2 -> setPlainText(sresult);
    }

    //Permutation 加密函数
    if(ui->tabWidget->currentIndex()==0 && ui->toolBox->currentIndex()==3 && ui->radioButton_9->isChecked()){
      //  this -> ui ->lineEdit -> setEnabled(false);
        char temp[500] = {0};
        QString qkey = this -> ui -> lineEdit -> text();
        string key = qkey.toStdString();
        QString qstr = this -> ui -> plainTextEdit_1->toPlainText();
        string st = qstr.toStdString();
        string str = encryPermutation(key, st, temp);
        QString sresult = QString::fromStdString(str);
        this -> ui -> plainTextEdit_2 -> setPlainText(sresult);
    }

    //Column 加密函数
    if(ui->tabWidget->currentIndex()==0 && ui->toolBox->currentIndex()==3 && ui->radioButton_10->isChecked()){
      //  this -> ui ->lineEdit -> setEnabled(false);
        char temp[500] = {0};
        QString qkey = this -> ui -> lineEdit -> text();
        string key = qkey.toStdString();
        QString qstr = this -> ui -> plainTextEdit_1->toPlainText();
        string st = qstr.toStdString();
        string str = encryColumn(key, st, temp);
        QString sresult = QString::fromStdString(str);
        this -> ui -> plainTextEdit_2 -> setPlainText(sresult);
    }
}


//解密
void frmMain::on_pushButton_13_clicked()
{
    //Caesar 解密函数
    if(ui->tabWidget->currentIndex()==0 && ui->toolBox->currentIndex()==0 && ui->radioButton_1->isChecked()){
        char temp[500] = {0};
        int key = this -> ui -> lineEdit -> text().toInt();
        QString qstr = this -> ui -> plainTextEdit_1->toPlainText();
        string st = qstr.toStdString();
        string str = decryCaesar(key, st, temp);
        QString sresult = QString::fromStdString(str);
        this -> ui -> plainTextEdit_2 -> setPlainText(sresult);
    }
    //Keyword  解密函数
    if(ui->tabWidget->currentIndex()==0 && ui->toolBox->currentIndex()==0 && ui->radioButton_2->isChecked()){
        char temp[500] = {0};
        QString qkey = this -> ui -> lineEdit -> text();
        string key = qkey.toStdString();
        QString qstr = this -> ui -> plainTextEdit_1->toPlainText();
        string st = qstr.toStdString();
        string str = encryVigenere(key, st, temp);
        QString sresult = QString::fromStdString(str);
        this -> ui -> plainTextEdit_2 -> setPlainText(sresult);
    }

    //Vigenere  解密函数
    if(ui->tabWidget->currentIndex()==0 && ui->toolBox->currentIndex()==1 && ui->radioButton_5->isChecked()){
        char temp[500] = {0};
        QString qkey = this -> ui -> lineEdit -> text();
        string key = qkey.toStdString();
        QString qstr = this -> ui -> plainTextEdit_1->toPlainText();
        string st = qstr.toStdString();
        string str = decryVigenere(key, st, temp);
        QString sresult = QString::fromStdString(str);
        this -> ui -> plainTextEdit_2 -> setPlainText(sresult);
    }

    //Autokey ciphertext 解密函数
    if(ui->tabWidget->currentIndex()==0 && ui->toolBox->currentIndex()==1 && ui->radioButton_6->isChecked()){
        char temp[500] = {0};
        QString qkey = this -> ui -> lineEdit -> text();
        string key = qkey.toStdString();
        QString qstr = this -> ui -> plainTextEdit_1->toPlainText();
        string st = qstr.toStdString();
        string str = decryAutokey(key, st, temp);
        QString sresult = QString::fromStdString(str);
        this -> ui -> plainTextEdit_2 -> setPlainText(sresult);
    }

    //Autokey plaintext 解密函数
    if(ui->tabWidget->currentIndex()==0 && ui->toolBox->currentIndex()==1 && ui->radioButton_7->isChecked()){
        char temp[500] = {0};
        QString qkey = this -> ui -> lineEdit -> text();
        string key = qkey.toStdString();
        QString qstr = this -> ui -> plainTextEdit_1->toPlainText();
        string st = qstr.toStdString();
        string str = decryAutokey2(key, st, temp);
        QString sresult = QString::fromStdString(str);
        this -> ui -> plainTextEdit_2 -> setPlainText(sresult);
    }

    //Playfair 解密函数
    if(ui->tabWidget->currentIndex()==0 && ui->toolBox->currentIndex()==2 && ui->radioButton_8->isChecked()){
      //this -> ui ->lineEdit -> setEnabled(false);
        char temp[500] = {0};
        QString qkey = this -> ui -> lineEdit -> text();
        string key = qkey.toStdString();
        QString qstr = this -> ui -> plainTextEdit_1->toPlainText();
        string st = qstr.toStdString();
        string str = decryPlayfair(key, st, temp);
        QString sresult = QString::fromStdString(str);
        this -> ui -> plainTextEdit_2 -> setPlainText(sresult);
    }

    //Permutation 解密函数
    if(ui->tabWidget->currentIndex()==0 && ui->toolBox->currentIndex()==3 && ui->radioButton_9->isChecked()){
      //  this -> ui ->lineEdit -> setEnabled(false);
        char temp[500] = {0};
        QString qkey = this -> ui -> lineEdit -> text();
        string key = qkey.toStdString();
        QString qstr = this -> ui -> plainTextEdit_1->toPlainText();
        string st = qstr.toStdString();
        string str = decryPermutation(key, st, temp);
        QString sresult = QString::fromStdString(str);
        this -> ui -> plainTextEdit_2 -> setPlainText(sresult);
    }
    //Column 解密函数
    if(ui->tabWidget->currentIndex()==0 && ui->toolBox->currentIndex()==3 && ui->radioButton_10->isChecked()){
      //  this -> ui ->lineEdit -> setEnabled(false);
        char temp[500] = {0};
        QString qkey = this -> ui -> lineEdit -> text();
        string key = qkey.toStdString();
        QString qstr = this -> ui -> plainTextEdit_1->toPlainText();
        string st = qstr.toStdString();
        string str = decryColumn(key, st, temp);
        QString sresult = QString::fromStdString(str);
        this -> ui -> plainTextEdit_2 -> setPlainText(sresult);
    }
}

//RC4加密
void frmMain::on_pushButton_clicked()
{
    int maxn=256+10;
    char source[maxn];

    //明文
    QString qstr = this -> ui -> plainTextEdit_3->toPlainText();
    QByteArray str= qstr.toLocal8Bit();
    strcpy(source,str);
    //密钥
    char pass[maxn];
    QString qkey = this -> ui -> lineEdit_3 -> text();
    QByteArray key= qkey.toLocal8Bit();
    strcpy(pass,key);
    //加密
    char* result = Encrypt_rc4(str,key);
    //密文
    QString sresult = QString::fromStdString(result);
    this -> ui -> plainTextEdit_4 -> setPlainText(sresult);
}

//RC4解密
void frmMain::on_pushButton_2_clicked()
{
    int maxn=256+10;
    char source[maxn];
    //密钥
    char pass[maxn];
    QString qkey = this -> ui -> lineEdit_3 -> text();
    QByteArray key= qkey.toLocal8Bit();
    strcpy(pass,key);
    //密文
    QString qstr = this -> ui -> plainTextEdit_3->toPlainText();
    QByteArray str= qstr.toLocal8Bit();
    strcpy(source,str);
    //解密
    char* result = Decrypt_rc4(str,key);
    QString sresult = QString::fromStdString(result);
    this -> ui -> plainTextEdit_4 -> setPlainText(sresult);
}

//DES加密
void frmMain::on_pushButton_3_clicked()
{
    char streamCipher[500] = {0};   //储存二进制密文
    char Cipher[500] = { 0 };   //储存字符密文
    char streamPlain[500] = { 0 };  //储存二进制明文
    string str1, str2, str3;
    QString s1, s2, s3;

    QString qkey = this -> ui -> lineEdit_2 -> text();
    string key = qkey.toStdString();
    QString qstr = this -> ui -> plainTextEdit->toPlainText();
    string st = qstr.toStdString();


    DES(key, st, streamCipher, Cipher, streamPlain);


    str1 = streamCipher;
    s1 = QString::fromStdString(str1);
    this -> ui -> plainTextEdit_6 -> setPlainText(s1);

    str2 = Cipher;
    s2 = QString::fromStdString(str2);
    this -> ui -> plainTextEdit_7 -> setPlainText(s2);



    str3 = streamPlain;
    s3 = QString::fromStdString(str3);
    this -> ui -> plainTextEdit_5 -> setPlainText(s3);
}

void frmMain::on_pushButton_5_clicked()
{

    unsigned long int flag;
    string s1, s2, s3, s4, s5;
    QString str1, str2, str3, str4, str5;
    char ch[500] = "";


    key(p,q,e,n,d);
    out_put(p, ch);
    s1 = ch;
    str1 = QString::fromStdString(s1);
    this -> ui ->plainTextEdit_14->setPlainText(str1);

    out_put(q, ch);
    s2 = ch;
    str2 = QString::fromStdString(s2);
    this -> ui ->plainTextEdit_15->setPlainText(str2);

    out_put(n, ch);
    s3 = ch;
    str3 = QString::fromStdString(s3);
    this -> ui ->plainTextEdit_16->setPlainText(str3);


    flag = e.value[0];
    itoa(flag, ch, 10);
    s4 = ch;
    str4 = QString::fromStdString(s4);
    this -> ui ->plainTextEdit_17->setPlainText(str4);

    out_put(d, ch);
    s5 = ch;
    str5 = QString::fromStdString(s5);
    this -> ui ->plainTextEdit_18->setPlainText(str5);

}

void frmMain::on_pushButton_6_clicked()
{
    string s6;
    QString str6;
    char ch[500] = "";
    QString qstr = this-> ui-> plainTextEdit_19->toPlainText();
    string st = qstr.toStdString();

    encryption(st, e, d, n,M,C, ch);
    out_put(C, ch);
    s6 = ch;
    str6 = QString::fromStdString(s6);
    this -> ui ->plainTextEdit_20->setPlainText(str6);

}

void frmMain::on_pushButton_7_clicked()
{
    QString qstr = this -> ui -> lineEdit_5 -> text();
    string str = qstr.toStdString();
    char *a = (char*)str.data();
    a = MD5String(a);
    QString qstr2 = QString::fromStdString(a);
    this -> ui -> lineEdit_6 -> setText(qstr2);

}

void frmMain::on_pushButton_8_clicked()
{
    char ch1[500] = "";
    char ch2[500] = "";
    unsigned long int flag;
    string s1, s2;
    QString str1, str2;

    generate(big_p, bg);
    out(big_p, ch1);
    s1 = ch1;
    str1 = QString::fromStdString(s1);
    this -> ui -> plainTextEdit_21 -> setPlainText(str1);

    flag = bg.value[0];
    itoa(flag, ch2, 10);
    //out_put(q, ch);
    s2 = ch2;
    str2 = QString::fromStdString(s2);
    this -> ui ->plainTextEdit_22->setPlainText(str2);
}

void frmMain::on_pushButton_9_clicked()
{
    string s1,st;
    QString str1,str;
    char ch[500] = {0};
    //int key = this->ui->plainTextEdit_2->toPlainText().toInt();
    str = this -> ui ->lineEdit_7 -> text();
    st = str.toStdString();
    send(A, A_send, st, big_p, bg);

    out(A_send, ch);
    cout << "Alice send: ";
    puts(ch);

    s1 = ch;
    str1 = QString::fromStdString(s1);
    this -> ui -> plainTextEdit_23 -> setPlainText(str1);
    this -> ui -> plainTextEdit_27 -> setPlainText(str1);
}

void frmMain::on_pushButton_11_clicked()
{
    string s1,st;
    QString str1,str;
    char ch[500] = {0};
    //int key = this->ui->plainTextEdit_2->toPlainText().toInt();
    str = this -> ui ->lineEdit_8 -> text();
    st = str.toStdString();
    send(B, B_send, st, big_p, bg);

    out(B_send, ch);
    cout << "Alice send: ";
    puts(ch);

    s1 = ch;
    str1 = QString::fromStdString(s1);
    this -> ui -> plainTextEdit_26 -> setPlainText(str1);
    this -> ui -> plainTextEdit_24 -> setPlainText(str1);
}

void frmMain::on_pushButton_10_clicked()
{
    char ch[500] ="";
    caculate(K1,B_send,A,big_p);
    out(K1, ch);
    printf("---------\n");
    puts(ch);
    string st = ch;
    QString str = QString::fromStdString(st);
    this -> ui ->plainTextEdit_25->setPlainText(str);
}

void frmMain::on_pushButton_14_clicked()
{
    char ch[500] ="";
    caculate(K2,A_send,B,big_p);
    out(K2, ch);
    printf("---------\n");
    puts(ch);
    string st = ch;
    QString str = QString::fromStdString(st);
    this -> ui ->plainTextEdit_28->setPlainText(str);
}

/*void frmMain::on_pushButton_Listen_clicked()
{
    if(ui->pushButton_Listen->text() == tr("侦听"))
    {
        //从输入框获取端口号
        int port = ui->lineEdit_Port->text().toInt();

        //监听指定的端口
        if(!server->listen(QHostAddress::Any, port))
        {
            //若出错，则输出错误信息
            qDebug()<<server->errorString();
            return;
        }
        //修改按键文字
        ui->pushButton_Listen->setText("取消侦听");
        qDebug()<< "Listen succeessfully!";
    }
    else
    {
        //如果正在连接
        if(socket->state() == QAbstractSocket::ConnectedState)
        {
            //关闭连接
            socket->disconnectFromHost();
        }
        //取消侦听
        server->close();
        //修改按键文字
        ui->pushButton_Listen->setText("侦听");
        //发送按键失能
        ui->pushButton_Send->setEnabled(false);
    }

}

void frmMain::on_pushButton_Send_clicked()
{
    qDebug() << "Send: " << ui->textEdit_Send->toPlainText();
    //获取文本框内容并以ASCII码形式发送
    socket->write(ui->textEdit_Send->toPlainText().toLatin1());
    socket->flush();
    //QString strData =QString::fromLocal8Bit("Time: ") + QTime::currentTime().toString() + "\n" + textEdit_Recv.toLocal8Bit() + "\n";
}

void frmMain::server_New_Connect()
{
    //获取客户端连接
    socket = server->nextPendingConnection();
    //连接QTcpSocket的信号槽，以读取新数据
    QObject::connect(socket, &QTcpSocket::readyRead, this, &frmMain::socket_Read_Data);
    QObject::connect(socket, &QTcpSocket::disconnected, this, &frmMain::socket_Disconnected);
    //发送按键使能
    ui->pushButton_Send->setEnabled(true);

    qDebug() << "A Client connect!";
}

void frmMain::socket_Read_Data()
{
    QByteArray buffer;
    //读取缓冲区数据
    buffer = socket->readAll();
    if(!buffer.isEmpty())
    {
        QString str = ui->textEdit_Recv->toPlainText();
        str+=tr(buffer);
        //刷新显示
        ui->textEdit_Recv->setText(str);
    }
}

void frmMain::socket_Disconnected()
{
    //发送按键失能
    ui->pushButton_Send->setEnabled(false);
    qDebug() << "Disconnected!";
}


//文件部分

/*--- 点击监听按钮 ---*/
//void frmMain::on_listenBtn_clicked()
//{
    /* 禁用监听按钮 */
/*  ui->listenBtn->setEnabled(false);
    if(!server->listen(QHostAddress::Any, PORT))
    {
        std::cerr << "*** Listen to Port Failed ***" << std::endl;
        qDebug() << server->errorString();
        close();
        ui->listenBtn->setEnabled(false);
        return;
    }
    ui->stLabel->setText(QString("Listing to Port %1").arg(PORT));
}

/*--- 接受连接请求 ---*/
/*void frmMain::accept_connect()
{
    receive = server->nextPendingConnection();
    /* 有数据到 -> 接受数据 */
    //connect(receive, SIGNAL(readyRead()),
      //      this, SLOT(recevie_file()));
    /* socket出错 -> 出错处理 */
/*    connect(receive, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(show_error(QAbstractSocket::SocketError)));
    ui->stLabel->setText(QString("Connection Established!"));
    gotBytes = 0;
    server->close();
}

/*--- 接收文件 ---*/
/*void frmMain::recevie_file()
{
    QDataStream in(receive);
    in.setVersion(DATA_STREAM_VERSION);

    /* 首部未接收/未接收完 */
 /*   if(gotBytes <= 2 * sizeof(qint64))
    {
        if(!nameSize) // 前两个长度字段未接收
        {
            if(receive->bytesAvailable() >= 2 * sizeof(qint64))
            {
                in >> fileBytes >> nameSize;
                gotBytes += 2 * sizeof(qint64);
                ui->recvProg->setMaximum(fileBytes);
                ui->recvProg->setValue(gotBytes);
            }
            else // 数据不足，等下次
               return;
        }
        else if(receive->bytesAvailable() >= nameSize)
        {
            in >> fileName;
            gotBytes += nameSize;
            ui->recvProg->setValue(gotBytes);
            std::cout << "--- File Name: "
                      << fileName.toStdString() << std::endl;
        }
        else // 数据不足文件名长度，等下次
            return;
    }

    /* 已读文件名、文件未打开 -> 尝试打开文件 */
 /*   if(!fileName.isEmpty() && file == Q_NULLPTR)
    {
        file = new QFile(fileName);
        if(!file->open(QFile::WriteOnly)) // 打开失败
        {
            std::cerr << "*** File Open Failed ***" << std::endl;
            delete file;
            file = Q_NULLPTR;
            return;
        }
        ui->stLabel->setText(QString("Open %1 Successfully!").arg(fileName));
    }
    if(file == Q_NULLPTR) // 文件未打开，不能进行后续操作
        return;

    if(gotBytes < fileBytes) // 文件未接收完
    {
        gotBytes += receive->bytesAvailable();
        ui->recvProg->setValue(gotBytes);
        file->write(receive->readAll());
    }
    if(gotBytes == fileBytes) // 文件接收完
    {
        receive->close(); // 关socket
        file->close(); // 关文件
        delete file;
        ui->stLabel->setText(QString("Finish receiving %1").arg(fileName));
        ui->listenBtn->setEnabled(true);
    }
}

/*--- 出错处理 ---*/
/*void frmMain::show_error(QAbstractSocket::SocketError)
{
    std::cerr << "*** Socket Error ***" << std::endl;
    qDebug() << receive->errorString();
    receive->close(); // 关cocket
    receive = Q_NULLPTR;
    file = Q_NULLPTR;
    fileName.clear(); // 清空文件名
    fileBytes = gotBytes = nameSize = 0;
    ui->recvProg->reset(); // 进度条归零
    ui->listenBtn->setEnabled(true); // 启用监听按钮
    ui->stLabel->setText(QString("*** SOCKET ERROR"));
}*/
    
    
    void frmMain::on_pushButton_Connect_clicked()
    {
        if(ui->pushButton_Connect->text() == tr("连接"))
        {
            QString IP;
            int port;
    
            //获取IP地址
            IP = ui->lineEdit_IP->text();
            //获取端口号
            port = ui->lineEdit_Port->text().toInt();
    
            //取消已有的连接
            socket->abort();
            //连接服务器
            socket->connectToHost(IP, port);
    
            //等待连接成功
            if(!socket->waitForConnected(30000))
            {
                qDebug() << "Connection failed!";
                return;
            }
            qDebug() << "Connect successfully!";
    
            //发送按键使能
            ui->pushButton_Send->setEnabled(true);
            //修改按键文字
            ui->pushButton_Connect->setText("断开连接");
        }
        else
        {
            //断开连接
            socket->disconnectFromHost();
            //修改按键文字
            ui->pushButton_Connect->setText("连接");
            ui->pushButton_Send->setEnabled(false);
        }
    }
    
    void frmMain::on_pushButton_Send_clicked()
    {
        qDebug() << "Send: " << ui->textEdit_Send->toPlainText();
         //获取文本框内容并以ASCII码形式发送
        socket->write(ui->textEdit_Send->toPlainText().toLatin1());
        socket->flush();
    }
    
    void frmMain::socket_Read_Data()
    {
        QByteArray buffer;
        //读取缓冲区数据
        buffer = socket->readAll();
        if(!buffer.isEmpty())
        {
            QString str = ui->textEdit_Recv->toPlainText();
            str+=tr(buffer);
            //刷新显示
            ui->textEdit_Recv->setText(str);
        }
    }
    
    void frmMain::socket_Disconnected()
    {
        //发送按键失能
        ui->pushButton_Send->setEnabled(false);
        //修改按键文字
        ui->pushButton_Connect->setText("连接");
        qDebug() << "Disconnected!";
    }









/*void frmMain::on_pushButton_16_clicked()
{
    char ch1[20] = "";
    char ch2[20] = "";
    string s1, s2;
    QString str1, str2;

    fun_pri(sim_p,sim_g);


    itoa(sim_p,ch1,10);
    itoa(sim_g,ch2,10);

    s1 = ch1;
    str1 = QString::fromStdString(s1);
    this -> ui -> lineEdit_9 -> setText(str1);

    s2 = ch2;
    str2 = QString::fromStdString(s2);
    this -> ui -> lineEdit_11-> setText(str2);
}*/

/*void frmMain::on_pushButton_17_clicked()
{
    string s1,st;
    QString str1,str;
    char ch[20] = {0};
    //int key = this->ui->plainTextEdit_2->toPlainText().toInt();
    a = this -> ui ->lineEdit_12 -> text().toInt();
    //st = str.toStdString();
    //send(A, A_send, st, big_p, bg);

    a_send = sim_caculate(sim_g, a, sim_p);

    itoa(a_send,ch,10);


    s1 = ch;
    str1 = QString::fromStdString(s1);
    this -> ui -> plainTextEdit_29 -> setPlainText(str1);
    //this -> ui -> plainTextEdit_27 -> setPlainText(str1);
}*/

/*void frmMain::on_pushButton_19_clicked()
{
    char ch[100] = {0};
    //QString str = this -> ui ->lineEdit_8 -> text();
    //string st = str.toStdString();
    b_send = this->ui->plainTextEdit_30->toPlainText().toInt();
    k1 = sim_caculate(b_send, a, sim_p);

    itoa(k1, ch, 10);
    string s1 = ch;
    QString str1 = QString::fromStdString(s1);
    this -> ui -> plainTextEdit_31 -> setPlainText(str1);
}
*/
/*void frmMain::on_pushButton_20_clicked()
{
    char temp[100] = {0};
    //k1 = sim_caculate(b_send, a, sim_p);
    //int key = this -> ui -> lineEdit -> text().toInt();
    QString qstr = this -> ui -> plainTextEdit_32->toPlainText();
    string st = qstr.toStdString();

    string str = encryCaesar_simple(k1, st,temp);
    QString sresult = QString::fromStdString(str);
    this -> ui -> plainTextEdit_33 -> setPlainText(sresult);
}*/

/*void frmMain::on_pushButton_22_clicked()
{
    char temp[100] = {0};
    //k1 = sim_caculate(b_send, a, sim_p);
    //int key = this -> ui -> lineEdit -> text().toInt();
    QString qstr = this -> ui -> plainTextEdit_34->toPlainText();
    string st = qstr.toStdString();

    string str = decryCaesar_simple(k1, st,temp);
    QString sresult = QString::fromStdString(str);
    this -> ui -> plainTextEdit_35 -> setPlainText(sresult);
}*/
