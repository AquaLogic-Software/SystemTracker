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

#ifndef DATEGRAPH_H
#define DATEGRAPH_H

#include "datagraph.h"

namespace Ui {
class DateGraph;
}

class DateGraph : public DataGraph
{
    Q_OBJECT

public:
    explicit DateGraph(QString name, Project *project, QString id = tr(""), QWidget *parent = 0);
    ~DateGraph();

    virtual void Replot();
private:
    Ui::DateGraph *ui;
};

#endif // DATEGRAPH_H
