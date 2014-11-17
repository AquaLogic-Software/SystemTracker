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

    class TableItem : public QTableWidgetItem
    {
    public:
        QString ContextID;
        QPair<int,int> Location;

        TableItem(QString id, int row, int col);
    };

    QMap<QString,QColor> GetTrackableIDs();

signals:

public slots:
    void SetTrackableIDs(QList<QString> ids);

private slots:
    void mousePressEvent(QMouseEvent *event);
    void onTrackableToggle(QString id);
    void onAddNew(DataEntry::Descriptor descriptor);
    void onEdit(DataEntry::Descriptor descriptor);
    void onCancelEdit();
    void onAddNewRequest();
    void onEditRequest(QString id);
    void onRemoveRequest(QString id);
    void ResetList();

private:

    Menu *menu;
    QColorDialog *colorDialog;
    QList<DataGraph::PlottableData> trackableData;
    QString contextID;

};

#endif // PLOTTABLEDATACHECKLIST_H
