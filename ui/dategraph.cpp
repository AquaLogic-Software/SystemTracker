/*
This file is part of AquaLife System Logger.

AquaLife System Logger is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

AquaLife System Logger is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with AquaLife System Logger.  If not, see <http://www.gnu.org/licenses/>
*/

#include "dategraph.h"
#include "ui_dategraph.h"

DateGraph::DateGraph(QString name, Project *project, QString id, QWidget *parent) :
    DataGraph(name, project, id, parent),
    ui(new Ui::DateGraph)
{
    ui->setupUi(this);

    this->setLayout(ui->mainLayout);

    this->graph->addGraph();
    this->graph->xAxis->setTickLabels(true);
    this->graph->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    this->graph->xAxis->setDateTimeFormat("MM/dd/yy");
    this->graph->xAxis->setLabel(name);
    this->graph->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    this->graph->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
    this->graph->xAxis->setAutoTickStep(true);
    this->graph->xAxis->setTickStep(604800); // one week in seconds
    this->graph->xAxis->setSubTickCount(4);
    this->graph->xAxis2->setVisible(true);
    this->graph->yAxis2->setVisible(true);

    uint now = QDateTime::currentDateTime().toTime_t();
    this->graph->xAxis->setRange(now-604800, now+604800);

    ui->mainLayout->addWidget(this->graph);

    if(id == tr(""))
        id = QUuid::createUuid().toString();
}

DateGraph::~DateGraph()
{
    delete ui;
    delete this->graph;
}

void DateGraph::Replot()
{
    this->graph->clearGraphs();

    for(int i = 0; i < this->graphCount; i++)
        this->graph->removeGraph(i);

    this->graphCount = 0;
    double high = 0;

    foreach (PlottableData plottable, this->plottableData)
    {
        this->graph->addGraph();

        QCPScatterStyle myScatter;
        myScatter.setShape(QCPScatterStyle::ssCircle);
        myScatter.setPen(plottable.Color);
        myScatter.setBrush(plottable.Color);
        myScatter.setSize(5);
        this->graph->graph(this->graphCount)->setScatterStyle(myScatter);

        QPen pen;
        pen.setColor(plottable.Color);
        this->graph->graph(this->graphCount)->setVisible(true);
        this->graph->graph(this->graphCount)->setLineStyle(QCPGraph::lsLine);
        this->graph->graph(this->graphCount)->setPen(pen);
        this->graph->graph(this->graphCount)->setBrush(QBrush(plottable.Color));

        QVector<double> time, data;
        time.clear();
        data.clear();
        //Very slow..needs replacement for beta..rushing a release
        foreach (Project::EntryList list, project->DataLists)
        {
            if(list.Descriptor.ID == plottable.Descriptor.ID)
            {
                this->graph->graph(this->graphCount)->setName(plottable.Descriptor.Name);

                foreach(DataEntry entry, list.DataList)
                {
                    time.append(entry.Timestamp.toTime_t());
                    data.append(entry.GetValue());

                    if(entry.GetValue() > high)
                        high = entry.GetValue();
                }

                this->graph->graph(this->graphCount)->setData(time,data);
                break;
            }
        }

        this->graphCount++;
    }

    this->graph->yAxis->setRange(0, high + 2);
    this->graph->replot();
    this->graph->legend->setVisible(true);
}
