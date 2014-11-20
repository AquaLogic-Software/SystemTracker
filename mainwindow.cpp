#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralWidget->setLayout(ui->mainLayout);
    ProjectManager::Init();

    startpage = new StartPage();
    this->connect(startpage, SIGNAL(ProjectOpenRequest(Project*)), SLOT(onProjectOpenRequest(Project*)));
    this->connect(startpage,SIGNAL(ProjectDeleteRequest(Project*)),SLOT(onProjectDeleteRequest(Project*)));
    ui->projectView->addTab(startpage, "Start Page");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    ProjectManager::SaveProjectList();

    for(int i = 0; i < ui->projectView->count(); i++)
        ui->projectView->widget(i)->close();
}

void MainWindow::onProjectCanceled()
{
    disconnectProjectSlots();
    this->newProjectCreator->hide();
    delete this->newProjectCreator;
}

void MainWindow::onProjectCreated(Project *project, QList<DataGraph *> graphs)
{
    disconnectProjectSlots();
    this->newProjectCreator->hide();
    delete this->newProjectCreator;

    ProjectView *view = new ProjectView(project);
    view->SaveView();
    view->OpenView();
    view->SetGraphs(graphs);

    ui->projectView->insertTab(ui->projectView->count(), view, project->Name);

    if(this->startpage != NULL)
    {
        ui->projectView->removeTab(0);
        delete this->startpage;
        this->startpage = NULL;
    }

    ui->projectView->setCurrentIndex(ui->projectView->count() - 1);
}

void MainWindow::onProjectOpenRequest(Project *project)
{
    ProjectManager::OpenProject(project);

    ProjectView *view = new ProjectView(project);
    ui->projectView->insertTab(ui->projectView->count(), view, project->Name);

    if(this->startpage != NULL)
    {
        ui->projectView->removeTab(0);
        delete this->startpage;
        this->startpage = NULL;
    }

    ui->projectView->setCurrentIndex(ui->projectView->count() - 1);
}

void MainWindow::onProjectDeleteRequest(Project *project)
{
}

void MainWindow::connectProjectSlots()
{
    this->connect(this->newProjectCreator, SIGNAL(ProjectCanceled()), SLOT(onProjectCanceled()));
    this->connect(this->newProjectCreator, SIGNAL(ProjectCreated(Project*,QList<DataGraph*>)), SLOT(onProjectCreated(Project*,QList<DataGraph*>)));
}

void MainWindow::disconnectProjectSlots()
{
    this->disconnect(this->newProjectCreator, SIGNAL(ProjectCanceled()), this, SLOT(onProjectCanceled()));
    this->disconnect(this->newProjectCreator, SIGNAL(ProjectCreated(Project*)), this, SLOT(onProjectCreated(Project*)));
}

void MainWindow::on_actionNew_Project_triggered()
{
    Project *project = new Project();
    this->newProjectCreator = new NewProjectWizzard(project,this);
    connectProjectSlots();
    this->newProjectCreator->show();
}
