#include "startpage.h"
#include "ui_startpage.h"

StartPage::StartPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartPage)
{
    ui->setupUi(this);
    this->setLayout(ui->mainLayout);

    for(int i = 0; i < 3 && i < ProjectManager::RecentProjects.count(); i++)
    {
        ProjectEntry *entry = new ProjectEntry(ProjectManager::RecentProjects.at(i));
        this->connect(entry, SIGNAL(ProjectDeleteRequest(Project*)), SLOT(requestDeleteProject(Project*)));
        this->connect(entry, SIGNAL(ProjectOpenRequest(Project*)), SLOT(requestOpenProject(Project*)));
        ui->mainLayout->insertWidget(i, entry);
    }
}

StartPage::~StartPage()
{
    delete ui;
}

void StartPage::requestOpenProject(Project *project)
{
    emit ProjectOpenRequest(project);
}

void StartPage::requestDeleteProject(Project *project)
{
    emit ProjectDeleteRequest(project);
}
