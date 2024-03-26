#include "registerwin.h"
#include "ui_registerwin.h"
#include"QFileDialog"、

#include"QSqlTableModel"
#include"QSqlRecord"
#include"QMessageBox"
#include"QDebug"
#include <QDir>
RegisterWin::RegisterWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegisterWin)
{
    ui->setupUi(this);
    //cap.open(0);
}

RegisterWin::~RegisterWin()
{
    delete ui;
}

void RegisterWin::timerEvent(QTimerEvent *e)
{
    //获取摄像头数据并显示
    if(cap.isOpened())
    {
        cap>>image;
        cv::Mat rgbimage;
        if(image.data == nullptr) return ;
        cv::cvtColor(image,rgbimage,cv::COLOR_BGR2RGB);
        QImage qmmp(rgbimage.data,rgbimage.cols,rgbimage.rows,rgbimage.step1(),QImage::Format_RGB888);
        QPixmap mmp=QPixmap::fromImage(qmmp);
        mmp.scaled(ui->headpiclb->size());
        ui->headpiclb->setPixmap(mmp);
    }
}

void RegisterWin::on_resetbtn_clicked()
{
    //清空数据
    ui->nameed->clear();
    ui->bired->clear();
    ui->departcb->clear();
    ui->worknumed->clear();
    ui->urled->clear();
    ui->bired->setDate(QDate::currentDate());

}

void RegisterWin::on_addimgbtn_clicked()
{
   //通过文件对话框选择路径
    QString filepath =QFileDialog::getOpenFileName(this);
    ui->urled->setText(filepath);

    //显示图片
    QPixmap mmp(filepath);
    mmp=mmp.scaledToWidth(ui->headpiclb->width());
    ui->headpiclb->setPixmap(mmp);
}

void RegisterWin::on_regbtn_clicked()
{
    //如果没有选图片，直接返回
    if(ui->urled->text()=="")
    {
        QMessageBox::warning(this,"注册提示","请添加图片");
        return;
    }
    //通过照片，结合faceObject模块得到faceid;

    cv::Mat image = cv::imread(ui->urled->text().toUtf8().data());
    faceid = faceobj.face_register(image);
    //把头像保存到一个固定路径下
    QDir dir(QDir::currentPath());
    if(!dir.exists("data"))
    {
        dir.mkdir("data");
        qDebug()<<QString("文件夹data创建成功！");
    }
    QString headfile = QString("./data/%1.jpg").arg(faceid);
    if(!cv::imwrite(headfile.toUtf8().data(),image))
    {
      QMessageBox::warning(this,"警告","图片插入路径错误");
    }

    //把个人信息存到数据库表employee
    QSqlTableModel model;
    model.setTable("employee");
    QSqlRecord record = model.record();
    record.setValue("name",ui->nameed->text());
    record.setValue("sex",ui->mrb->isChecked()?"男":"女");
    record.setValue("birthday",ui->bired->text());
    record.setValue("worknumber",ui->worknumed->text());
    record.setValue("depart",ui->departcb->currentText());
    record.setValue("faceid",faceid);
    //头像路径
    record.setValue("headfile",headfile);

    //把记录插入到表中
    bool ret=model.insertRecord(0,record);
    //提示注册成功
    if(ret)
    {
       QMessageBox::information(this,"注册提示","注册成功");
       //提交
       model.submitAll();
    }
    else
    {
        QMessageBox::information(this,"注册提示","注册失败");
    }
    faceobj.flush();
}

void RegisterWin::on_camera_btn_clicked()
{
    if(ui->camera_btn->text()=="打开摄像头")
    {   //打开摄像头
        if(cap.open(0))
        {
            ui->camera_btn->setText("关闭摄像头");
            //启动定时器，采集数据
            timerid=startTimer(100);
        }
    }
    else{

        ui->camera_btn->setText("打开摄像头");
        killTimer(timerid);
        ui->headpiclb->clear();
        cap.release();
    }
}

void RegisterWin::on_actionbtn_clicked()
{
    ui->camera_btn->setText("打开摄像头");
    killTimer(timerid);
    QString headfile = QString("./data/%1.jpg").arg(faceid);
    ui->urled->setText(headfile);
    cv::imwrite(headfile.toUtf8().data(),image);
    cap.release();
}
