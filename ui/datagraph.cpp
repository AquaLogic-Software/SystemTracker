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

QList<DataGraph::PlottableData> DataGraph::GetPlottableData()
{
    return this->plottableData;
}

QList<QString> DataGraph::GetPlottableIDs()
{
    QList<QString> ids;
    ids.clear();

    foreach(PlottableData data, this->plottableData)
        ids.append(data.Descriptor.ID);

    return ids;
}

void DataGraph::SetPlottableData(QList<DataGraph::PlottableData> plottable)
{
    this->plottableData.clear();
    this->plottableData.append(plottable);
}

void DataGraph::AddPlottable(DataGraph::PlottableData plottable)
{
    this->plottableData.append(plottable);
}

void DataGraph::SetRange(QDateTime from, QDateTime to)
{
    this->range.first = from;
    this->range.second = to;

    this->graph->xAxis->setRange(from.toTime_t(), to.toTime_t());
}
