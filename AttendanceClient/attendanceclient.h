#ifndef ATTENDANCECLIENT_H
#define ATTENDANCECLIENT_H

#include <QMainWindow>
#include<opencv.hpp>
#include<QTcpSocket>
#include<QTimer>
using namespace std;
using namespace cv;
QT_BEGIN_NAMESPACE
namespace Ui { class AttendanceClient; }
QT_END_NAMESPACE

class AttendanceClient : public QMainWindow
{
    Q_OBJECT

public:
    AttendanceClient(QWidget *parent = nullptr);
    ~AttendanceClient();
    void timerEvent(QTimerEvent *e);
private slots:
    void timer_connect();
    void stop_connect();
    void start_connect();
    void read_data();

private:
    Ui::AttendanceClient *ui;
    void reset();
    //摄像头
    VideoCapture cap;
    //haar--级联分类器
    cv::CascadeClassifier cascade;
    //创建网络套接字，定时器
    QTcpSocket ms;
    QTimer mt;
    cv::Mat curface;
    //标志是否是同一个人脸进入识别区
    int flag;

};
#endif // ATTENDANCECLIENT_H
