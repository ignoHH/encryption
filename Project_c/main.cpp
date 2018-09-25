#include "frmmain.h"
#include <QApplication>
#include "myhelper.h"
#include "mydialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    myHelper::SetUTF8Code();
    myHelper::SetStyle("black");//黑色风格
//    myHelper::SetStyle("blue");//蓝色风格
//    myHelper::SetStyle("gray");//灰色风格
//    myHelper::SetStyle("navy");//天蓝色风格
    myHelper::SetChinese();

    MyDialog dialog;
    frmMain w;
    dialog.setWindowTitle(QObject::tr("登陆"));
    w.setWindowTitle("工程实践");
    if(dialog.exec() == QDialog::Accepted){
        w.show();
        return a.exec();
    }

    else return 0;
}
