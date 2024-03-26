#include "attendancetable.h"
#include "ui_attendancetable.h"
#include"QDebug"
attendanceTable::attendanceTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::attendanceTable)
{
    ui->setupUi(this);
    model.setTable("attendance");
    model.select();
    model.setHeaderData(0,Qt::Horizontal, "id");
    model.setHeaderData(1,Qt::Horizontal, "考勤人id");
    model.setHeaderData(2,Qt::Horizontal, "考勤人姓名");
    model.setHeaderData(3,Qt::Horizontal, "打卡时间");
    ui->tableView->setModel(&model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

attendanceTable::~attendanceTable()
{
    delete ui;
}

void attendanceTable::on_pushButton_clicked()
{

    QString doc=ui->lineEdit->text();
    if(doc=="")
    {
        model.select();
    }
    QString filterQS=" name like '%"+doc+"%'";
    model.setFilter(filterQS);

}
