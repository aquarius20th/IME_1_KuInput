#include <QtGui/QApplication>
#include <QTextCodec>
#include "keyboard.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForTr( QTextCodec::codecForName("GB2312") );    //在移植时可能出现编码问题
    keyboard w;
    w.show();
    return a.exec();
}
