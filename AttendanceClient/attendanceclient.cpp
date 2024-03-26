#include "attendanceclient.h"
#include "ui_attendanceclient.h"
#include "QDebug"
#include "QJsonDocument"
#include "QJsonParseError"
#include "QJsonObject"
#include"QFile"
AttendanceClient::AttendanceClient(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AttendanceClient)
{
    ui->setupUi(this);
    //打开摄像头
    cap.open(0);

    //启动定时器
    startTimer(100);
    //导入级联分类器文件

    cascade.load("D:/opencv452/etc/haarcascades/haarcascade_frontalface_alt2.xml");

    //QTcpSocket 断开连接时候发送disconnected信号，连接成功 connected
    connect(&ms,&QTcpSocket::disconnected,this,&AttendanceClient::start_connect);
    connect(&ms,&QTcpSocket::connected,this,&AttendanceClient::stop_connect);
    ui->widget_2->hide();
    //关联接受数据的槽函数
    connect(&ms,&QTcpSocket::readyRead,this,&AttendanceClient::read_data);
    //定时连接服务器
    connect(&mt,&QTimer::timeout,this,&AttendanceClient::timer_connect);
    mt.start(1000);
    flag=0;
}

AttendanceClient::~AttendanceClient()
{
    delete ui;
}

void AttendanceClient::timerEvent(QTimerEvent *e)
{
    //采集数据
    Mat srcImage;
    if(cap.grab())
    {
        cap.read(srcImage);//读取一帧数据
    }
    cv::resize(srcImage,srcImage,Size(480,480));
    Mat grayImage;
    //转灰度图
    cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);
    //检测人脸数据
    std::vector<Rect> faceRects;
    cascade.detectMultiScale(grayImage,faceRects);//检测人脸
    if(faceRects.size()>0 && flag>=0)
    {

        Rect rect = faceRects.at(0);//第一个人脸的矩形框
        // rectangle(srcImage,rect,Scalar(0,0,255));
        //移动人脸框（图片--QLabel）
        ui->headpicLb->move(rect.x/2-10,rect.y/2);

        if(flag > 2){
            //把Mat数据转化为QbyteArray， --》编码成jpg格式
            std::vector<uchar> buf;
            cv::imencode(".jpg",srcImage,buf);
            QByteArray byte((const char*)buf.data(),buf.size());
            //准备发送
            quint64 backsize = byte.size();
            QByteArray sendData;
            QDataStream stream(&sendData,QIODevice::WriteOnly);
            stream.setVersion(QDataStream::Qt_5_14);
            stream<<backsize<<byte;
            //发送
            ms.write(sendData);

            bool exist = QFile::exists("./face.jpg");
            if(exist)
            {
                QFile::remove("./face.jpg");
            }
            imwrite("./face.jpg",srcImage(rect));
            flag = -2;

        }
        flag++;
    }
    if(faceRects.size() == 0)
    {
        //把人脸框移动到中心位置
        ui->headpicLb->move(100,60);
        flag=0;
        reset();
    }

    if(srcImage.data == nullptr) return;
    //把opencv里面的Mat格式数据（BGR）转Qt里面的QImage(RGB)
    cvtColor(srcImage,srcImage, COLOR_BGR2RGB);
    QImage image(srcImage.data,srcImage.cols, srcImage.rows,srcImage.step1(),QImage::Format_RGB888);
    QPixmap mmp = QPixmap::fromImage(image);

    ui->vedioLb->setPixmap(mmp);
}

void AttendanceClient::timer_connect()
{
    //连接服务器
    ms.connectToHost("192.168.92.1",9999);
    qDebug()<<"正在连接服务器";
}

void AttendanceClient::stop_connect()
{
    mt.stop();
    qDebug()<<"停止连接";
}

void AttendanceClient::start_connect()
{
    mt.start(5000);
    qDebug()<<"连接成功";
}

void AttendanceClient::read_data()
{
  QByteArray msg=ms.readAll();
  //Json解析
  QJsonParseError err;
  QJsonDocument doc=QJsonDocument::fromJson(msg,&err);
  if(err.error!=QJsonParseError::NoError)
  {
      //json数据出错
      return;
  }
  QJsonObject obj=doc.object();
  QString worknumber = obj.value("worknum").toString();
  QString name = obj.value("name").toString();
  QString depart = obj.value("depart").toString();
  QString time=obj.value("curtime").toString();
  if(worknumber !="-1")
  {
      ui->worknumLb->setText(worknumber);
      ui->nameLb->setText(name);
      ui->departLb->setText(depart);
      ui->timeLb->setText(time);

      QPixmap pixmap;
         pixmap.load("./face.jpg");
      //通过样式来显示图片
      ui->headLb->setStyleSheet("border-radius:65;border-image: url(: ./face.jpg);");
      pixmap=pixmap.scaled(ui->headLb->size());
      ui->headLb->setPixmap(pixmap);
      ui->widget_2->show();
  }
}

void AttendanceClient::reset()
{
    ui->worknumLb->clear();
    ui->nameLb->clear();
    ui->departLb->clear();
    ui->timeLb->clear();
    ui->headLb->clear();
}

