#ifndef MAINWIN_H
#define MAINWIN_H

#include <QMainWindow>
#include"registerwin.h"
#include"attendanceserver.h"
#include"attendancetable.h"
#include"employeetable.h"
namespace Ui {
class MainWin;
}

class MainWin : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWin(QWidget *parent = nullptr);
    ~MainWin();

private slots:
    void on_mainW_clicked();

    void on_regW_clicked();

    void on_usersW_clicked();

    void on_attW_clicked();

    void on_stackedWidget_currentChanged(int arg1);

private:
    Ui::MainWin *ui;
    RegisterWin *resW;
    attendanceTable *attTableW;
    AttendanceServer *attSerW;
    employeeTable *empTableW;
};

#endif // MAINWIN_H
