#include "datagraph.h"

DataGraph::DataGraph(QString name, Project *project, QString id, QObject *parent) :
    QFrame(parent)
{
    this->identification = QPair<QString,QString>(name, id);
    this->plottableData.clear();
    this->graph = new QCustomPlot(this);
    this->graphCount = 0;
    this->project = project;
}

QString DataGraph::GetName()
{
    return this->identification.first;
}

const QString DataGraph::GetID()
{
    return (const QString)this->identification.second;
}

const QMap<QString, QColor> DataGraph::GetlottableData()
{
    return(const QMap<QString, QColor>)this->plottableData;
}

QList<DataGraph::PlottableData> DataGraph::GetPlottableIDs()
{
    QList<QString> ids;
    ids.clear();

    QMap<QString,QColor>::iterator i = this->plottableData.begin();
    while(i != this->plottableData.end())
        ids.append(i.key());

    return ids;
}

void DataGraph::AddPlottable(QString ID, QColor color)
{
    this->plottableData.insert(ID, color);
}

void DataGraph::SetRange(QDateTime from, QDateTime to)
{
    this->range.first = from;
    this->range.second = to;

    this->graph->xAxis->setRange(from.toTime_t(), to.toTime_t());
}
