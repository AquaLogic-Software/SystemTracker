#ifndef TRACKABLEDATACHECKLIST_H
#define TRACKABLEDATACHECKLIST_H

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QMouseEvent>
#include <QAction>
#include <QMenu>
#include "../dataentry.h"
#include "../project.h"
#include "datagraph.h"
#include "menu.h"

#include "datadescriptoreditor.h"

class TrackableDataChecklist : public QTableWidget
{
    Q_OBJECT

public:

    class TableItem : public QTableWidgetItem
    {
    public:
        QString ContextID;
        QPair<int,int> Location;

        TableItem(QString id, int row, int col);
    };

    explicit TrackableDataChecklist(QObject *parent = 0);
    TrackableDataChecklist(Project *project, QObject *parent = 0);

    ~TrackableDataChecklist();

    QList<QString> GetTrackableIDs();

signals:

public slots:
    void SetTrackableIDs(QList<QString> ids);

private slots:
    void mousePressEvent(QMouseEvent *event);
    void onAddNew(DataEntry::Descriptor descriptor);
    void onEdit(DataEntry::Descriptor descriptor);
    void onCancelEdit();
    void onAddNewRequest(QVariant data);
    void onEditRequest(QVariant data);
    void onRemoveRequest(QVariant data);
    void onTrackableToggle(QVariant data);
    void ResetList();

private:

    Menu *menu;
    DataDescriptorEditor *editor;
    QList<QString> trackableData;
    Project *project;
};

#endif // TRACKABLEDATACHECKLIST_H
