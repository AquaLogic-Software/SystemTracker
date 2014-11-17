#ifndef DATAGRAPH_H
#define DATAGRAPH_H

#include <QFrame>
#include <QPair>
#include <QMap>
#include <QUuid>

#include "../project.h"
#include "3rd_party/qcustomplot/qcustomplot.h"
#include "layoutdata.h"

class DataGraph : public QFrame
{
    Q_OBJECT
public:

    struct PlottableData
    {
        DataEntry::Descriptor Descriptor;
        QColor Color;
    };

    explicit DataGraph(QString name, Project *project, QString id = tr(""), QObject *parent = 0);

    void AddPlottable(QString ID, QColor color);
    void SetRange(QDateTime from, QDateTime to);
    virtual void Replot() = 0;
    QString GetName();
    const QString GetID();
    const QMap<QString,QColor> GetlottableData();
    QList<PlottableData> GetPlottableIDs();

    LayoutData Layout;

protected:
    QCustomPlot *graph;
    Project *project;

    QPair<QString,QString> identification;
    QPair<QDateTime,QDateTime> range;
    QList<PlottableData> plottableData;

    int graphCount;

signals:

public slots:

};

#endif // DATAGRAPH_H
