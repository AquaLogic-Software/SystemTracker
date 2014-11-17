#include "projectentry.h"
#include "ui_projectentry.h"

ProjectEntry::ProjectEntry(Project *project, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectEntry)
{
    ui->setupUi(this);
    this->setLayout(ui->mainLayout);

    ui->titleLabel->setText(project->Name);
    ui->descriptionLabel->setText(project->Description);

    this->project = project;
}

ProjectEntry::~ProjectEntry()
{
    delete ui;
}

void ProjectEntry::on_pushButton_3_clicked()
{
    emit ProjectOpenRequest(this->project);
}

void ProjectEntry::on_deleteProjectButton_clicked()
{
    emit ProjectDeleteRequest(this->project);
}
