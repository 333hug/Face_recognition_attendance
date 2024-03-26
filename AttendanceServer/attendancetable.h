#ifndef ATTENDANCETABLE_H
#define ATTENDANCETABLE_H

#include <QWidget>
#include "QSqlTableModel"
namespace Ui {
class attendanceTable;
}

class attendanceTable : public QWidget
{
    Q_OBJECT

public:
    explicit attendanceTable(QWidget *parent = nullptr);
    ~attendanceTable();

private slots:
    void on_pushButton_clicked();

private:
    Ui::attendanceTable *ui;
    QSqlTableModel model;
};

#endif // ATTENDANCETABLE_H
