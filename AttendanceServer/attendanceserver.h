#ifndef ATTENDANCESERVER_H
#define ATTENDANCESERVER_H
#include"qfaceobject.h"
#include <QMainWindow>
#include<QTcpServer>
#include<QTcpSocket>
#include"QSqlTableModel"
#include"QSqlRecord"
namespace Ui {
class AttendanceServer;
}

class AttendanceServer : public QMainWindow
{
    Q_OBJECT

public:
    explicit AttendanceServer(QWidget *parent = nullptr);
    ~AttendanceServer();
private slots:
    void new_client();
    void read_Data();
    void recv_faceid(int64_t faceid);
signals:
    void query(cv::Mat &image);
private:
    Ui::AttendanceServer *ui;
    QTcpServer mserver;
    QTcpSocket *msocket;
    quint64 bsize;
    QFaceObject fobj;
    QSqlTableModel model;
};

#endif // ATTENDANCESERVER_H
