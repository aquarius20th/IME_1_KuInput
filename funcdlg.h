#ifndef FUNCDLG_H
#define FUNCDLG_H

#include <QtGui/QWidget>

namespace Ui {
    class funcDlg;
}

class funcDlg : public QWidget {
    Q_OBJECT
public:
    funcDlg(QWidget *parent = 0);
    ~funcDlg();
    Ui::funcDlg *m_ui;

signals:
    void homeBtn_clicked();
    void endBtn_clicked();
    void delBtn_clicked();
    void insertBtn_clicked();
    void inputMethodBtn_clicked();
    void pageDownBtn_clicked();
    void pageUpBtn_clicked();
    void noLkBtn_clicked();
    void prtScBtn_clicked();
    void scLkBtn_clicked();

protected:
    void changeEvent(QEvent *e);



private slots:
    void on_scLkBtn_clicked();
    void on_noLkBtn_clicked();
    void on_prtScBtn_clicked();
    void on_pageDownBtn_clicked();
    void on_pageUpBtn_clicked();
    void on_inputMethodBtn_clicked();
    void on_insertBtn_clicked();
    void on_delBtn_clicked();
    void on_endBtn_clicked();
    void on_homeBtn_clicked();
};

#endif // FUNCDLG_H
