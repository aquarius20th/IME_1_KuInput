#include "funcdlg.h"
#include "ui_funcdlg.h"

funcDlg::funcDlg(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::funcDlg)
{
    m_ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

funcDlg::~funcDlg()
{
    delete m_ui;
}

void funcDlg::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void funcDlg::on_homeBtn_clicked()
{
    emit homeBtn_clicked();
}

void funcDlg::on_endBtn_clicked()
{
    emit endBtn_clicked();
}

void funcDlg::on_delBtn_clicked()
{
    emit delBtn_clicked();
}

void funcDlg::on_insertBtn_clicked()
{
    emit insertBtn_clicked();
}

void funcDlg::on_inputMethodBtn_clicked()
{
    emit inputMethodBtn_clicked();
}

void funcDlg::on_pageUpBtn_clicked()
{
    emit pageUpBtn_clicked();
}

void funcDlg::on_pageDownBtn_clicked()
{
    emit pageDownBtn_clicked();
}

void funcDlg::on_prtScBtn_clicked()
{
    emit prtScBtn_clicked();
}

void funcDlg::on_noLkBtn_clicked()
{
    emit noLkBtn_clicked();
}

void funcDlg::on_scLkBtn_clicked()
{
    emit scLkBtn_clicked();
}
