#include <QtGui/QApplication>
#include <QTextCodec>
#include "keyboard.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForTr( QTextCodec::codecForName("GB2312") );    //����ֲʱ���ܳ��ֱ�������
    keyboard w;
    w.show();
    return a.exec();
}
