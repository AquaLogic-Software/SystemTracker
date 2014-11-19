#ifndef NEWPROJECTWIZZARD_H
#define NEWPROJECTWIZZARD_H

#include <QDialog>
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>
#include <QInputDialog>
#include <QVector>
#include <QCloseEvent>

#include "trackabledatachecklist.h"
#include "plottabledatachecklist.h"
#include "../project.h"
#include "dategraph.h"

namespace Ui {
class NewProjectWizzard;
}

class NewProjectWizzard : public QDialog
{
    Q_OBJECT

    enum State
    {
        Details,
        Graphs,
        Finished
    } state = Details;

public:
    explicit NewProjectWizzard(Project *project, QWidget *parent = 0);
    ~NewProjectWizzard();

    void closeEvent(QCloseEvent *event);

private slots:
    void on_buttonBox_accepted();
    void on_locationButton_clicked();
    void on_buttonBox_rejected();

    void Advance();

    void on_addGraphButton_clicked();

    void on_graphSelectorCombo_currentIndexChanged(int index);

signals:
    void ProjectCreated(Project*,QList<DataGraph*>);
    void ProjectCanceled();

private:
    Ui::NewProjectWizzard *ui;
    Project *project;
    QVector<DataGraph*> graphs;
    TrackableDataChecklist *checkList;
    PlottableDataCheckList *graphCheckList;
    int graphIndex;
};

#endif // NEWPROJECTWIZZARD_H
