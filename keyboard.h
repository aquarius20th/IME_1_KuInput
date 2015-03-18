#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QtGui/QDialog>
#include <QMouseEvent>
#include <QLineEdit>
#include <QMessageBox>
#include <QFile>
#include <QRegExp>

#include "chinesebar.h"
#include "funcdlg.h"

static char keyMap[2][5][14]=
{
        {
            {'\e','`','1','2','3','4','5','6','7','8','9','0','-','='},
            {'\t','\t','q','w','e','r','t','y','u','i','o','p','\\','\b'},
            {'\C','\C','a','s','d','f','g','h','j','k','l',';','\n','\n'},
            {'\f','\f','z','x','c','v','b','n','m',',',' ','.','\'','/'},
            {'1','2','3',' ',' ',' ',' ','[',']','\'','\t','\t','\a','\a'}
        },
        {
            {' ','~','!','@','#','$','%','^','&','*','(',')','_','+'},
            {'\t','\t','Q','W','E','R','T','Y','U','I','O','P','|','\b'},
            {'\C','\C','A','S','D','F','G','H','J','K','L',':','\n','\n'},
            {'\f','\f','Z','X','C','V','B','N','M','<',' ','>','\"','?'},
            {' ',' ',' ',' ',' ',' ',' ','{','}',' ',' ',' ','\a','\a'}
        },
};

enum inputMethod{english,chinese};

namespace Ui
{
    class keyboard;
}


class keyboard : public QDialog
{
    Q_OBJECT

public:
    keyboard(QWidget *parent = 0);
    ~keyboard();

private:
    Ui::keyboard *ui;
    funcDlg *func;
    chineseBar *bar;
    bool isShiftDown;
    bool isCapsDown;
    bool isFuncActive;
    bool findNothing;
    inputMethod method;
    int flagKey;
    int currentPage;
    QKeyEvent* transkey(int x,int y);
    QString transPy(QString pyInput);   //根据输入的拼音转换成汉字
    QString py;
    QLineEdit *keyRev;
    QFile file;
    QFile out;
    QRegExp regExp;
    void changeKeyboard();  //更换键盘大小写布局
    void on_func_clicked(int fx,int fy);
    void showPage(int index);
    int pageCount;

private slots:
    void changeInputMethod(); //更换输入法类型english or chinese
    void chineseSelectFont_1();
    void chineseSelectFont_2();
    void chineseSelectFont_3();
    void chineseSelectFont_4();
    void chineseSelectFont_5();
    void chineseSelectFont_6();
    void chineseSelectFont_7();
    void chineseSelectFont_8();
    void pageUp();
    void pageDown();

protected:
    virtual bool eventFilter(QObject *obj, QEvent *event);

};

#endif // KEYBOARD_H
