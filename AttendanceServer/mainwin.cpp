#include "mainwin.h"
#include "ui_mainwin.h"

MainWin::MainWin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWin)
{
    ui->setupUi(this);
    resW=new RegisterWin();
    attTableW=new attendanceTable();
    empTableW = new employeeTable();
    attSerW=new AttendanceServer();

    ui->stackedWidget->addWidget(resW);
    ui->stackedWidget->addWidget(attSerW);
    ui->stackedWidget->addWidget(attTableW);
    ui->stackedWidget->addWidget(empTableW);
}

MainWin::~MainWin()
{
    delete ui;
    delete resW;
    delete attTableW;
    delete attSerW;
    delete empTableW;
}

void MainWin::on_mainW_clicked()
{
    on_stackedWidget_currentChanged(1);
}

void MainWin::on_regW_clicked()
{
    on_stackedWidget_currentChanged(0);

}

void MainWin::on_usersW_clicked()
{
    on_stackedWidget_currentChanged(3);

}

void MainWin::on_attW_clicked()
{
    on_stackedWidget_currentChanged(2);

}

void MainWin::on_stackedWidget_currentChanged(int arg1)
{
    ui->stackedWidget->setCurrentIndex(arg1);
}
