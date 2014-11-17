#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralWidget->setLayout(ui->mainLayout);
    ProjectManager::Init();

    StartPage *startpage = new StartPage();
    this->connect(startpage, SIGNAL(ProjectOpenRequest(Project*)), SLOT(onProjectOpenRequest(Project*)));
    this->connect(startpage,SIGNAL(ProjectDeleteRequest(Project*)),SLOT(onProjectDeleteRequest(Project*)));
    ui->projectView->addTab(startpage, "Start Page");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onProjectCanceled()
{
    disconnectProjectSlots();
    this->newProjectCreator->hide();
    delete this->newProjectCreator;
}

void MainWindow::onProjectCreated(Project *project)
{
    disconnectProjectSlots();
    this->newProjectCreator->hide();
    delete this->newProjectCreator;

    ProjectView *view = new ProjectView(project);
    ui->projectView->insertTab(ui->projectView->count(), view, project->Name);
}

void MainWindow::onProjectOpenRequest(Project *project)
{
    ProjectManager::OpenProject(project);

    ProjectView *view = new ProjectView(project);
    ui->projectView->insertTab(ui->projectView->count(), view, project->Name);
}

void MainWindow::onProjectDeleteRequest(Project *project)
{
}

void MainWindow::connectProjectSlots()
{
    this->connect(this->newProjectCreator, SIGNAL(ProjectCanceled()), SLOT(onProjectCanceled()));
    this->connect(this->newProjectCreator, SIGNAL(ProjectCreated(Project*)), SLOT(onProjectCreated(Project*)));
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
