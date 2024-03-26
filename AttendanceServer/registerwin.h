#ifndef REGISTERWIN_H
#define REGISTERWIN_H
#include"qfaceobject.h"
#include <QWidget>
#include <opencv.hpp>
namespace Ui {
class RegisterWin;
}
using namespace cv;
class RegisterWin : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterWin(QWidget *parent = nullptr);
    ~RegisterWin();
    void timerEvent(QTimerEvent *e);

private slots:
    void on_resetbtn_clicked();

    void on_addimgbtn_clicked();

    void on_regbtn_clicked();

    void on_camera_btn_clicked();

    void on_actionbtn_clicked();

private:
    Ui::RegisterWin *ui;
    int timerid;
    VideoCapture cap;
    cv::Mat image;
    int64_t faceid;
    QFaceObject faceobj;
};

#endif // REGISTERWIN_H
