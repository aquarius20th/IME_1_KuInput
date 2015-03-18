#ifndef CHINESEBAR_H
#define CHINESEBAR_H

#include <QtGui/QWidget>

namespace Ui {
    class chineseBar;
}

class chineseBar : public QWidget {
    Q_OBJECT
public:
    chineseBar(QWidget *parent = 0);
    ~chineseBar();
    Ui::chineseBar *m_ui;

protected:
    void changeEvent(QEvent *e);

};

#endif // CHINESEBAR_H
