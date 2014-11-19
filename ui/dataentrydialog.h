#ifndef DATAENTRYDIALOG_H
#define DATAENTRYDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QMouseEvent>
#include <QAction>
#include <QMenu>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QCloseEvent>

#include "../dataentry.h"
#include "../project.h"

namespace Ui {
class DataEntryDialog;
}

class DataEntryDialog : public QDialog
{
    Q_OBJECT

    class TableItem : public QTableWidgetItem
    {
    public:
        QString ContextID;
        QPair<int,int> Location;

        TableItem(QString id, int row, int col);
    };

public:
    explicit DataEntryDialog(Project *project, QWidget *parent = 0);
    ~DataEntryDialog();

    virtual void closeEvent(QCloseEvent *event);

signals:
    void UserFinished(bool);

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::DataEntryDialog *ui;
    Project *project;
    QTableWidget *entryTable;
};

#endif // DATAENTRYDIALOG_H
