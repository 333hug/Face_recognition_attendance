#include "qfaceobject.h"

QFaceObject::QFaceObject(QObject *parent) : QObject(parent)
{

    //初始化
    seeta::ModelSetting FDmodel("D:/seetaface/data/fd_2_00.dat",seeta::ModelSetting::CPU,0);
    seeta::ModelSetting PDmodel("D:/seetaface/data/pd_2_00_pts5.dat",seeta::ModelSetting::CPU,0);
    seeta::ModelSetting FRmodel("D:/seetaface/data/fr_2_10.dat",seeta::ModelSetting::CPU,0);
    this->fengineptr = new seeta::FaceEngine(FDmodel,PDmodel,FRmodel);

    //导入已有的人脸数据库
    this->fengineptr->Load("./face.db");
}

QFaceObject::~QFaceObject()
{
    delete fengineptr;
}

int64_t QFaceObject::face_register(cv::Mat &faceImage)
{
    SeetaImageData simage;
    simage.data=faceImage.data;
    simage.width=faceImage.cols;
    simage.height=faceImage.rows;
    simage.channels=faceImage.channels();

    int64_t faceid =this->fengineptr->Register(simage);
    if(faceid>=0)
    {
        fengineptr->Save("./face.db");
    }

    return faceid;
}

int QFaceObject::face_query(cv::Mat &faceImage)
{
    SeetaImageData simage;
    simage.data=faceImage.data;
    simage.width=faceImage.cols;
    simage.height=faceImage.rows;
    simage.channels=faceImage.channels();

    float similarity=0;
    int64_t faceid=fengineptr->Query(simage,&similarity);//运行时间比较长
    if(similarity>0.7)
    {
        emit send_id(faceid);
    }
    else
    {
         emit send_id(-1);
    }
    return faceid;
}

void QFaceObject::flush()
{
     this->fengineptr->Load("./face.db");
}
