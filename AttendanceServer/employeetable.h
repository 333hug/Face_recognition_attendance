#ifndef EMPLOYEETABLE_H
#define EMPLOYEETABLE_H

#include <QWidget>
#include "QSqlTableModel"
namespace Ui {
class employeeTable;
}

class employeeTable : public QWidget
{
    Q_OBJECT

public:
    explicit employeeTable(QWidget *parent = nullptr);
    ~employeeTable();

private slots:
    void on_pushButton_clicked();

private:
    Ui::employeeTable *ui;
    QSqlTableModel model;
};

#endif // EMPLOYEETABLE_H
