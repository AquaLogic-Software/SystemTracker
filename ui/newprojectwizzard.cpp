#include "newprojectwizzard.h"
#include "ui_newprojectwizzard.h"

NewProjectWizzard::NewProjectWizzard(Project *project, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewProjectWizzard)
{
    ui->setupUi(this);
    this->setLayout(ui->mainLayout);
    this->project = project;
    this->graphs.clear();
    this->checkList = new TrackableDataChecklist(this);
    this->ui->trackableDataLayout->insertWidget(0, this->checkList);
    this->ui->graphSetupWidget->setVisible(false);
    this->ui->graphCheckListWidget->setLayout(this->ui->graphCheckListLayout);
    this->ui->projectSetupWidget->setLayout(ui->projectSetupLayout);
    this->ui->graphSetupWidget->setLayout(ui->graphSetupLayout);
    this->ui->mainLayout->insertWidget(0, this->ui->projectSetupWidget);

    this->graphIndex = -1;
}

NewProjectWizzard::~NewProjectWizzard()
{
    delete ui;
}

void NewProjectWizzard::closeEvent(QCloseEvent *event)
{
    emit ProjectCanceled();
}

void NewProjectWizzard::on_buttonBox_accepted()
{
    if(this->state == Details)
    {
        bool okay = true;
        QStringList missing;

        if(ui->nameEdit->text().isNull() || ui->nameEdit->text().isEmpty())
        {
            okay = false;
            missing.append("Name");
        }

        if(ui->locationEdit->text().isNull() || ui->locationEdit->text().isEmpty())
        {
            okay = false;
            missing.append("Location");
        }

        if(!okay)
        {
            QString missingText("The following information is required, but missing:\n\n");
            foreach(QString addition, missing)
                missingText.append(addition + QString("\n"));

            QMessageBox::warning(this, tr("New Project"),missingText,QMessageBox::Ok);

            return;
        }

        this->project->Name = ui->nameEdit->text();
        this->project->ID = ProjectManager::GenerateUUID();
        this->project->Description = ui->descriptionEdit->toPlainText();

        QList<DataEntry::Descriptor> descriptorList = ProjectManager::GetDescriptors();
        QList<QString> idList = this->checkList->GetTrackableIDs();

        //this->project->
        foreach(DataEntry::Descriptor descriptor, descriptorList)
        {
            if(idList.contains(descriptor.ID))
            {
                this->project->TrackableDescriptors.append(descriptor);
                this->project->DataLists.append(Project::EntryList(descriptor));
            }
        }

        this->project->Location = ui->locationEdit->text() + QDir::separator() + this->project->Name;
        this->Advance();
    }
    else if(this->state == Graphs)
    {
        this->Advance();
    }
}

void NewProjectWizzard::on_locationButton_clicked()
{
    QFileDialog dirSelector(this);
    dirSelector.setFileMode(QFileDialog::Directory);
    dirSelector.setOption(QFileDialog::ShowDirsOnly, true);
    int ret = dirSelector.exec();

    if(ret == QFileDialog::Rejected)
        return;

    ui->locationEdit->setText(dirSelector.selectedFiles().at(0));
}

void NewProjectWizzard::on_buttonBox_rejected()
{
    delete this->project;
    emit ProjectCanceled();
}

void NewProjectWizzard::Advance()
{
    switch(this->state)
    {
    case Details:
        this->state = Graphs;

        this->graphCheckList = new PlottableDataCheckList(this);
        this->graphCheckList->SetTrackable(this->project->GetTrackableIDs());
        this->ui->graphCheckListLayout->insertWidget(0,this->graphCheckList);

        this->ui->mainLayout->removeWidget(this->ui->projectSetupWidget);
        this->ui->projectSetupWidget->setVisible(false);
        this->ui->projectSetupWidget->setEnabled(false);
        this->ui->graphSetupWidget->setVisible(true);
        this->ui->graphSetupWidget->setEnabled(true);
        this->ui->mainLayout->insertWidget(0, this->ui->graphSetupWidget);
        break;

    case Graphs:
        QDir dir(this->project->Location);

        if(!dir.mkpath(this->project->Location + QDir::separator() + QString("images")))
        {
            QMessageBox::warning(this, tr("New Project"),tr("Could not create directory"),QMessageBox::Ok);
            return;
        }

        ProjectManager::SaveProject(this->project);
        emit ProjectCreated(this->project, this->graphs.toList());
        break;
    }
}

void NewProjectWizzard::on_addGraphButton_clicked()
{
    bool okay;
    QString text = QInputDialog::getText
    (
        this,
        tr("Set Graph Name"),
        tr("Graph Name:"), QLineEdit::Normal,
        tr("New Graph"),
        &okay
    );

    if(okay)
    {
        this->graphs.append(new DateGraph(text, this->project, ProjectManager::GenerateUUID()));
        this->ui->graphSelectorCombo->insertItem(this->graphs.count() - 1, text);
    }
}

void NewProjectWizzard::on_graphSelectorCombo_currentIndexChanged(int index)
{
    if(this->graphIndex != -1)
        this->graphs.at(graphIndex)->SetPlottableData(this->graphCheckList->GetPlottable());

    this->graphCheckList->SetPlottable(this->graphs.at(index)->GetPlottableData());
    this->graphIndex = index;
}
