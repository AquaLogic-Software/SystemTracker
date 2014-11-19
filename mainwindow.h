#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "ui/projectentry.h"
#include "projectview.h"
#include "ui/newprojectwizzard.h"
#include "project.h"
#include "startpage.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected slots:
    void onProjectCanceled();
    void onProjectCreated(Project *project, QList<DataGraph*> graphs);
    void onProjectOpenRequest(Project *project);
    void onProjectDeleteRequest(Project *project);
    void connectProjectSlots();
    void disconnectProjectSlots();

private slots:
    void on_actionNew_Project_triggered();

private:
    Ui::MainWindow *ui;
    NewProjectWizzard *newProjectCreator;
    StartPage *startpage;
};

#endif // MAINWINDOW_H
