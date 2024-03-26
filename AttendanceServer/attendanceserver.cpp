#include "attendanceserver.h"
#include "ui_attendanceserver.h"
#include"QDateTime"
#include"QThread"
AttendanceServer::AttendanceServer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AttendanceServer)
{

    ui->setupUi(this);
    connect(&mserver,&QTcpServer::newConnection,this,&AttendanceServer::new_client);

    mserver.listen(QHostAddress::Any,9999);//监听

    bsize=0;

    model.setTable("employee");
    QThread *thread = new QThread();
    //把QFaceobject对象移动到thread线程中执行
    fobj.moveToThread(thread);
    //启动线程
    thread->start();
    connect(this,&AttendanceServer::query,&fobj,&QFaceObject::face_query);

    //关联QFaceObject里的send_id信号
    connect(&fobj,&QFaceObject::send_id,this,&AttendanceServer::recv_faceid);
}

AttendanceServer::~AttendanceServer()
{
    delete ui;
}

void AttendanceServer::new_client()
{
    //获取客户端套接字
    msocket = mserver.nextPendingConnection();

    //当有客户端达到是会发送readyread信号

    connect(msocket,&QTcpSocket::readyRead,this,&AttendanceServer::read_Data);
}

void AttendanceServer::read_Data()
{
    QDataStream stream(msocket);//把套接字绑定到数据流
    stream.setVersion(QDataStream::Qt_5_14);

    if(bsize==0)
    {
        if(msocket->bytesAvailable()<(qint64)sizeof(bsize)) return;

        //采集数据长度
        stream>>bsize;
    }
    if(msocket->bytesAvailable()<bsize)
    {
        return;
    }

    QByteArray data;
    stream>>data;

    bsize=0;
    if(data.size()==0)//没有读取到数据
        return;

    //显示图片
    QPixmap mmp;
    mmp.loadFromData(data,"jpg");
    mmp=mmp.scaled(ui->piclb->size());
    ui->piclb->setPixmap(mmp);

    //识别人脸
    cv::Mat faceImage;
    std::vector<uchar>decode;
    decode.resize(data.size());
    memcpy(decode.data(),data.data(),data.size());

    faceImage=cv::imdecode(decode,cv::IMREAD_COLOR);

    emit query(faceImage);//int faceid=fobj.face_query(faceImage);

}

void AttendanceServer::recv_faceid(int64_t faceid)
{

    if(faceid<0)
    {
        QString sdmsg = QString("{\"worknum\":\"-1\",\"name\":\"-1\",\"depart\":\"-1\",\"curtime\":\"-1\"}");
        msocket->write(sdmsg.toUtf8());
        return ;
    }
    //从数据库中查询faceid的人员信息
    model.setFilter(QString("faceid = %1").arg(faceid));

    model.select();

    if(model.rowCount()==1)
    {
      //工号，姓名，部门，时间
      //以json格式返回
        QSqlRecord record = model.record(0);
        QString sendmsg=QString("{\"worknum\":\"%1\",\"name\":\"%2\",\"depart\":\"%3\",\"curtime\":\"%4\"}")
                .arg(record.value("worknumber").toString())
                .arg(record.value("name").toString())
                .arg(record.value("depart").toString())
                .arg(QDateTime::currentDateTime().toString("yyyy-mm-dd hh:mm:ss"));
        msocket->write(sendmsg.toUtf8());


        //把考勤信息存到数据库表attendance
        QSqlTableModel model2;
        model2.setTable("attendance");
        QSqlRecord record2 = model2.record();
        record2.setValue("employeeid",record.value("employeeid").toInt());
        record2.setValue("name",record.value("name").toString());
        record2.setValue("attendancetime",QDateTime::currentDateTime().toString("yyyy-m-dd hh:mm:ss"));
        //把记录插入到表中
        if( !model2.insertRecord(0,record2))
        {
            qDebug()<<record2;
        }
        if(! model2.submitAll())
        {
            qDebug()<<1;
        }

    }

}
