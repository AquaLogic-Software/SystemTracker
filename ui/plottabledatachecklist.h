#ifndef PLOTTABLEDATACHECKLIST_H
#define PLOTTABLEDATACHECKLIST_H

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QMouseEvent>
#include <QAction>
#include <QMenu>
#include <QColorDialog>
#include "../dataentry.h"
#include "../project.h"
#include "datagraph.h"

#include "datadescriptoreditor.h"
#include "trackabledatachecklist.h"

class PlottableDataCheckList : public QTableWidget
{
    Q_OBJECT
public:
    explicit PlottableDataCheckList(QObject *parent = 0);
    ~PlottableDataCheckList();

    class TableItem : public QTableWidgetItem
    {
    public:
        QString ContextID;
        QPair<int,int> Location;

        TableItem(QString id, int row, int col);
    };

    QList<DataGraph::PlottableData> GetPlottable();
    void SetPlottable(QList<DataGraph::PlottableData> plottable);
    void SetTrackable(QList<QString> trackable);

signals:

private slots:
    void mousePressEvent(QMouseEvent *event);
    void onEditRequest(QVariant data);
    void onTrackableToggle(QVariant data);
    void ResetList();

private:

    Menu *menu;
    QColorDialog *colorDialog;
    QList<DataGraph::PlottableData> plottableData;
    QList<QString> trackableData;
    QString contextID;

};

#endif // PLOTTABLEDATACHECKLIST_H
