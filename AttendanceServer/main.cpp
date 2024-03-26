#include "attendanceserver.h".h"

#include <QApplication>
#include<QSqlDatabase>
#include<QSqlError>
#include<QSqlQuery>
#include<opencv.hpp>
#include"mainwin.h"
#include"registerwin.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<cv::Mat>("cv::Mat&");
    qRegisterMetaType<cv::Mat>("cv::Mat");
    qRegisterMetaType<int64_t>("int64_t");

    //连接数据库
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("server.db");
    if(!db.open())
    {
        qDebug()<<db.lastError().text();
        return -1;
    }
    //创建员工信息表表格
    QString createsql = "create table if not exists employee(employeeid integer primary key autoincrement,"
                        "name varchar(256),sex varchar(32),birthday text,depart text,"
                        "worknumber text,faceid integer unique,headfile text)";
    QSqlQuery query(db);
    if(!query.exec(createsql))
    {
        qDebug()<<db.lastError().text();
        return -1;
    }
    //考勤表格
    createsql = "create table if not exists attendance(attendanceid integer primary key autoincrement,"
                        "employeeid integer,name varchar(10),attendancetime text)";
    if(!query.exec(createsql))
    {
        qDebug()<<db.lastError().text();
        return -1;
    }

//    RegisterWin s;
//    s.show();
//    AttendanceServer w;
//    w.show();
    MainWin w;
    w.show();
    return a.exec();
}
