#include "employeetable.h"
#include "ui_employeetable.h"

employeeTable::employeeTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::employeeTable)
{
    ui->setupUi(this);
    model.setTable("employee");
    model.select();
    model.setHeaderData(0,Qt::Horizontal, "id");
    model.setHeaderData(1,Qt::Horizontal, "姓名");
    model.setHeaderData(2,Qt::Horizontal, "性别");
    model.setHeaderData(3,Qt::Horizontal, "出生日期");
    model.setHeaderData(4,Qt::Horizontal, "院系");
    model.setHeaderData(5,Qt::Horizontal, "工号");
    model.setHeaderData(6,Qt::Horizontal, "人脸id");
    model.setHeaderData(7,Qt::Horizontal, "人脸保存路径");

    ui->tableView->setModel(&model);



}

employeeTable::~employeeTable()
{
    delete ui;
}

void employeeTable::on_pushButton_clicked()
{
    QString doc=ui->lineEdit->text();
    if(doc=="")
    {
        model.select();
    }
    QString filterQS=" name like '%"+doc+"%'";
    model.setFilter(filterQS);
}
