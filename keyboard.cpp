#include "keyboard.h"
#include "ui_keyboard.h"
#include "ui_funcdlg.h"
#include "ui_chinesebar.h"

#include <QDebug>

keyboard::keyboard(QWidget *parent)
    : QDialog(parent), ui(new Ui::keyboard)
{
    ui->setupUi(this);

    this->setWindowFlags(this->windowFlags()|Qt::WindowStaysOnTopHint);
    this->installEventFilter(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_InputMethodEnabled);
    this->setFocusPolicy(Qt::NoFocus);
    this->setFocus();


    this->setStyleSheet("background-image: url(:/keyboard/src/lowKeyboard.JPG);");

    isShiftDown = false;  
    isCapsDown = false;
    isFuncActive = false;
    findNothing = true;

    pageCount = 0;

    bar = new chineseBar(this);
    bar->setGeometry(0,0,311,20);
    bar->setStyleSheet("background-image: url();");
    connect(bar->m_ui->font_1,SIGNAL(clicked()),this,SLOT(chineseSelectFont_1()));
    connect(bar->m_ui->font_2,SIGNAL(clicked()),this,SLOT(chineseSelectFont_2()));
    connect(bar->m_ui->font_3,SIGNAL(clicked()),this,SLOT(chineseSelectFont_3()));
    connect(bar->m_ui->font_4,SIGNAL(clicked()),this,SLOT(chineseSelectFont_4()));
    connect(bar->m_ui->font_5,SIGNAL(clicked()),this,SLOT(chineseSelectFont_5()));
    connect(bar->m_ui->font_6,SIGNAL(clicked()),this,SLOT(chineseSelectFont_6()));
    connect(bar->m_ui->font_7,SIGNAL(clicked()),this,SLOT(chineseSelectFont_7()));
    connect(bar->m_ui->font_8,SIGNAL(clicked()),this,SLOT(chineseSelectFont_8()));
    connect(bar->m_ui->listUp,SIGNAL(clicked()),this,SLOT(pageUp()));
    connect(bar->m_ui->listDown,SIGNAL(clicked()),this,SLOT(pageDown()));

    func = new funcDlg();
    connect(func,SIGNAL(inputMethodBtn_clicked()),this,SLOT(changeInputMethod()));
    func->hide();

    flagKey = 0;    //0Сд���� | 1��д����
    keyRev = new QLineEdit();

   // QLineEdit *keyRev2 = new QLineEdit();
   // QLineEdit *keyRev3 = new QLineEdit();
   // keyRev2->show();
   // keyRev3->show();

    keyRev->show();
    setFocusProxy(QApplication::focusWidget());

    file.setFileName(":/keyboard/src/py.txt");
    out.setFileName("serch.txt");
    if( !out.open(QIODevice::ReadWrite|QIODevice::Truncate) )
        QMessageBox::warning(0,tr("��ʼ������"),tr("�޷�д���ѯ���ݣ������ļ��Ƿ���Ч��"));
    if( !file.open(QIODevice::ReadOnly) )
        QMessageBox::warning(0,tr("���ش���"),tr("�޷������ַ����룬�����ļ��Ƿ���Ч��"));


    regExp.setPatternSyntax(QRegExp::RegExp);
    regExp.setCaseSensitivity(Qt::CaseSensitive);

    currentPage = 0;

}

keyboard::~keyboard()
{
    delete ui;
}

QKeyEvent* keyboard::transkey(int x ,int y)
{
    int kx, ky;
    char c, text[2];
    QKeyEvent *key;

    if(y < 16)
         return NULL;

    kx = (y-16)/16;
    ky = x/22;


    c = keyMap[flagKey][kx][ky];

        if(c == '\b')				//BackSpace
        {
                key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);
        }
        else if(c == '\n')			//Enter
        {
                key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier);
        }
        else if(c == '\C')			//CapsLock
        {
                key = new QKeyEvent(QEvent::KeyPress, Qt::Key_CapsLock, Qt::NoModifier);

        }
        else if(c == '\e')			//ESC
        {
                key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Escape, Qt::NoModifier);

        }
        else if(c == '\t')			//Tab
        {
                key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Tab, Qt::NoModifier);

        }
        else if(c == '\f')			//Shift
        {
            key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Shift, Qt::NoModifier);
            isShiftDown = !isShiftDown;
            changeKeyboard();
        }
        else if(c == '\a')			//Func
        {
                on_func_clicked(x,y);   //����Ϊfunc��ť���������꣬�Բ�����Ӧ��menu
                return NULL;
        }
        else
        {
                c = keyMap[flagKey][kx][ky];
                sprintf(text, "%c\0", c);
                QString ch(text);
                key = new QKeyEvent(QEvent::KeyPress, 0, Qt::NoModifier, ch);

        }
        return key;









}

void keyboard::on_func_clicked(int fx,int fy)
{
    func->setGeometry(fx+this->x()-110,fy+this->y()-91,110,91);
    if(!isFuncActive){
        func->show();
        isFuncActive = true;
    }else{
        func->hide();
        isFuncActive = false;
    }



}

void keyboard::changeKeyboard()
{
    if(isShiftDown)
    {
        this->setStyleSheet("background-image: url(:/keyboard/src/highKeyboard.JPG);");
        flagKey = 1;
    }else{
        this->setStyleSheet("background-image: url(:/keyboard/src/lowKeyboard.JPG);");
        flagKey = 0;
    }


}

void keyboard::changeInputMethod()
{
    if( method == english)   //english
    {
        method = chinese;   //change to chinese
        func->m_ui->inputMethodBtn->setText(tr("��"));


    }else{
        method = english;   //change to english
        func->m_ui->inputMethodBtn->setText(tr("Ӣ"));

    }

}

QString keyboard::transPy(QString pyInput)
{

    py.append(pyInput);
    bar->m_ui->lineEdit->insert(pyInput);

    out.open(QIODevice::ReadWrite|QIODevice::Truncate);
      
    int ret = 0;
    int count = 0;  //ͳ��ƥ����ַ��������Խ���ÿҳ��ʾ���ַ�
    regExp.setPattern(QString("([^a-z\\s]{2,8})(%1)(\\s)").arg(py));//��Ϊ���ı����к���д��ĸ,Ϊ���֣��ʲ��ô�Сд����
    while(!file.atEnd())
    {
        QByteArray data = file.readLine();
        ret = regExp.indexIn(QString(data.data()),0,QRegExp::CaretAtZero);
         if( ret > -1)   //����ҵ�ƥ��������ַ�
        {
            findNothing = false;
            out.write(regExp.cap(1).toAscii().data());         
            out.write("\n");
            count++;

        }

    }
    file.seek(0);
    out.close();


    if(count%8 == 0)
    {
        pageCount = count/8;
    }else{
        pageCount = (count/8)+1;
    }

    if(findNothing)    //���û��ƥ��������ַ�
    {
        bar->m_ui->font_1->setText("");
        bar->m_ui->font_2->setText("");
        bar->m_ui->font_3->setText("");
        bar->m_ui->font_4->setText("");
        bar->m_ui->font_5->setText("");
        bar->m_ui->font_6->setText("");
        bar->m_ui->font_7->setText("");
        bar->m_ui->font_8->setText("");
        return NULL;

    }

    //����ҵ�ƥ��������ַ�
    findNothing = true;
    showPage(0);    //��ʾ��һҳƥ����ַ�,��0��ʼ
    //return NULL;
    return py;

}

void keyboard::showPage(int index)
{
    int n =0;
    QString chinese[8];

    out.open(QIODevice::ReadWrite);
    while(!out.atEnd())
    {
        for(int j=0;j<8;j++)
        {
            QByteArray data = out.readLine();
            data.replace("\n","\0");
            n++;
            if((index*8)< n && n <((index+1)*8)+1)
            {
                chinese[j].append(data);
            }
       }
    }

    bar->m_ui->font_1->setText(tr(chinese[0].toAscii()));
    bar->m_ui->font_2->setText(tr(chinese[1].toAscii()));
    bar->m_ui->font_3->setText(tr(chinese[2].toAscii()));
    bar->m_ui->font_4->setText(tr(chinese[3].toAscii()));
    bar->m_ui->font_5->setText(tr(chinese[4].toAscii()));
    bar->m_ui->font_6->setText(tr(chinese[5].toAscii()));
    bar->m_ui->font_7->setText(tr(chinese[6].toAscii()));
    bar->m_ui->font_8->setText(tr(chinese[7].toAscii()));

    out.close();


}

void keyboard::pageUp()
{
    currentPage--;
    if(currentPage > -1)
    {
        bar->m_ui->listDown->setEnabled(true);
        if(currentPage-- == 0)
            bar->m_ui->listUp->setDisabled(true);
        currentPage++;
        showPage(currentPage);
    }else{
        currentPage++;  //�ص�֮ǰ��ֵ
        bar->m_ui->listUp->setDisabled(true);
    }

}

void keyboard::pageDown()
{
    currentPage++;
    if(currentPage < pageCount)
    {
         bar->m_ui->listUp->setEnabled(true);
         if(currentPage++ == pageCount-1)
            bar->m_ui->listDown->setDisabled(true);
         currentPage--;
         showPage(currentPage);
     }else{
        currentPage--;
        bar->m_ui->listDown->setDisabled(true);
     }


}

void keyboard::chineseSelectFont_1()
{
    keyRev->insert(bar->m_ui->font_1->text());
    py.clear();
    bar->m_ui->lineEdit->clear();
    bar->m_ui->font_1->setText("");
    bar->m_ui->font_2->setText("");
    bar->m_ui->font_3->setText("");
    bar->m_ui->font_4->setText("");
    bar->m_ui->font_5->setText("");
    bar->m_ui->font_6->setText("");
    bar->m_ui->font_7->setText("");
    bar->m_ui->font_8->setText("");
}

void keyboard::chineseSelectFont_2()
{
    keyRev->insert(bar->m_ui->font_2->text());
    py.clear();
    bar->m_ui->lineEdit->clear();
    bar->m_ui->lineEdit->clear();
    bar->m_ui->font_1->setText("");
    bar->m_ui->font_2->setText("");
    bar->m_ui->font_3->setText("");
    bar->m_ui->font_4->setText("");
    bar->m_ui->font_5->setText("");
    bar->m_ui->font_6->setText("");
    bar->m_ui->font_7->setText("");
    bar->m_ui->font_8->setText("");
}

void keyboard::chineseSelectFont_3()
{
    keyRev->insert(bar->m_ui->font_3->text());
    py.clear();
    bar->m_ui->lineEdit->clear();
    bar->m_ui->lineEdit->clear();
    bar->m_ui->font_1->setText("");
    bar->m_ui->font_2->setText("");
    bar->m_ui->font_3->setText("");
    bar->m_ui->font_4->setText("");
    bar->m_ui->font_5->setText("");
    bar->m_ui->font_6->setText("");
    bar->m_ui->font_7->setText("");
    bar->m_ui->font_8->setText("");
}

void keyboard::chineseSelectFont_4()
{
    keyRev->insert(bar->m_ui->font_4->text());
    py.clear();
    bar->m_ui->lineEdit->clear();
    bar->m_ui->lineEdit->clear();
    bar->m_ui->font_1->setText("");
    bar->m_ui->font_2->setText("");
    bar->m_ui->font_3->setText("");
    bar->m_ui->font_4->setText("");
    bar->m_ui->font_5->setText("");
    bar->m_ui->font_6->setText("");
    bar->m_ui->font_7->setText("");
    bar->m_ui->font_8->setText("");
}

void keyboard::chineseSelectFont_5()
{
    keyRev->insert(bar->m_ui->font_5->text());
    py.clear();
    bar->m_ui->lineEdit->clear();
    bar->m_ui->lineEdit->clear();
    bar->m_ui->font_1->setText("");
    bar->m_ui->font_2->setText("");
    bar->m_ui->font_3->setText("");
    bar->m_ui->font_4->setText("");
    bar->m_ui->font_5->setText("");
    bar->m_ui->font_6->setText("");
    bar->m_ui->font_7->setText("");
    bar->m_ui->font_8->setText("");
}

void keyboard::chineseSelectFont_6()
{
    keyRev->insert(bar->m_ui->font_6->text());
    py.clear();
    bar->m_ui->lineEdit->clear();
    bar->m_ui->lineEdit->clear();
    bar->m_ui->font_1->setText("");
    bar->m_ui->font_2->setText("");
    bar->m_ui->font_3->setText("");
    bar->m_ui->font_4->setText("");
    bar->m_ui->font_5->setText("");
    bar->m_ui->font_6->setText("");
    bar->m_ui->font_7->setText("");
    bar->m_ui->font_8->setText("");
}

void keyboard::chineseSelectFont_7()
{
    keyRev->insert(bar->m_ui->font_7->text());
    py.clear();
    bar->m_ui->lineEdit->clear();
    bar->m_ui->lineEdit->clear();
    bar->m_ui->font_1->setText("");
    bar->m_ui->font_2->setText("");
    bar->m_ui->font_3->setText("");
    bar->m_ui->font_4->setText("");
    bar->m_ui->font_5->setText("");
    bar->m_ui->font_6->setText("");
    bar->m_ui->font_7->setText("");
    bar->m_ui->font_8->setText("");
}

void keyboard::chineseSelectFont_8()
{
    keyRev->insert(bar->m_ui->font_8->text());
    py.clear();
    bar->m_ui->lineEdit->clear();
    bar->m_ui->lineEdit->clear();
    bar->m_ui->font_1->setText("");
    bar->m_ui->font_2->setText("");
    bar->m_ui->font_3->setText("");
    bar->m_ui->font_4->setText("");
    bar->m_ui->font_5->setText("");
    bar->m_ui->font_6->setText("");
    bar->m_ui->font_7->setText("");
    bar->m_ui->font_8->setText("");
}


bool keyboard::eventFilter(QObject *obj, QEvent *event)
{
        QKeyEvent *key;


        if(1)
        {
                if(event->type() == QEvent::MouseButtonPress)
                {
                        QMouseEvent *mouse = (QMouseEvent *)event;

                        if((key = transkey(mouse->x(), mouse->y())) != NULL)
                        {

                                if( method == english)  //�����Ӣ�����뷨
                            {
                                QCoreApplication::postEvent(QApplication::focusWidget(), key);
                                return true;

                            }else{  //������������뷨

                                QString c = key->text();       //���س�modifer����ַ�

                            //    if(!c.contains('\b',Qt::CaseSensitive)) //�������ת���ַ�����������                                
                                if( transPy(c) != NULL )
                                {

                                    return true;
                                }


                                return false;   //���û�ж�Ӧ������

                            }


                        }


                }
        }

        return false;
}

/*
void keyboard::mousePressEvent(QMouseEvent *event)
{
    QKeyEvent *key;

    if(event->type() == QEvent::MouseButtonPress)
    {
       if ((key = transkey(event->x(),event->y())) != NULL)
        {
                //QCoreApplication::postEvent(this, key);
                QCoreApplication::postEvent(keyRev, key);

        }

    }

}
*/
