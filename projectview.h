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

#ifndef PROJECTVIEW_H
#define PROJECTVIEW_H

#include <QWidget>
#include <QString>
#include <QList>
#include <QDate>
#include <QMap>
#include <QDir>
#include <QFile>
#include <QApplication>
#include <QtXml/QtXml>
#include <QUuid>
#include <QMap>
#include <QRgb>

#include "3rd_party/qcustomplot/qcustomplot.h"
#include "ui/dategraph.h"
#include "project.h"
#include "ui/layoutdata.h"
#include "ui/dataentrydialog.h"

namespace Ui {
class ProjectView;
}

class ProjectView : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectView(Project *project, QWidget *parent = 0);
    ~ProjectView();

    void ReplotGraphs();
    void SaveView();
    void OpenView();
    void SetGraphs(QList<DataGraph*> graphs);

private slots:

    void on_applyRangeButton_clicked();
    void on_addEntryButton_clicked();
    void UserFinishedEntry(bool newData);

private:
    Ui::ProjectView *ui;
    DataEntryDialog *entryDialog;

    Project *project;
    QList<DataGraph*> graphs;
};

#endif // PROJECTVIEW_H
