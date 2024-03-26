#ifndef QFACEOBJECT_H
#define QFACEOBJECT_H
#include<seeta/FaceEngine.h>
#include <QObject>
#include<opencv.hpp>
//人脸数据存储，人脸检测，人脸识别
class QFaceObject : public QObject
{
    Q_OBJECT
public:
    explicit QFaceObject(QObject *parent = nullptr);
    virtual ~QFaceObject();

signals:
public slots:
    int64_t face_register(cv::Mat &faceImage);
    int face_query(cv::Mat &faceImage);
    void flush();
signals:
    void send_id(int64_t faceid);
private:
    seeta::FaceEngine *fengineptr;
};

#endif // QFACEOBJECT_H
