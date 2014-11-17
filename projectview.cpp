#include "projectview.h"
#include "ui_projectview.h"

ProjectView::ProjectView(Project *project, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectView)
{
    ui->setupUi(this);
    this->project = project;

    this->setLayout(ui->mainLayout);
    ui->graphTab->setLayout(ui->graphTabLayout);
    ui->textTab->setLayout(ui->textViewLayout);

    this->OpenView();

    foreach(DateGraph *graph, this->graphs)
        ui->graphTabLayout->addWidget(graph);
}

ProjectView::~ProjectView()
{
    delete ui;
}

void ProjectView::ReplotGraphs()
{
    foreach(DateGraph *graph, this->graphs)
        graph->Replot();
}

void ProjectView::SaveView()
{
    QDir dir(project->Location);
    QFile file(dir.absoluteFilePath(project->Name + QString(".apv")));

    if(!file.open(QFile::WriteOnly))
    {
        qWarning() << "Could not create file " << file.fileName();
        return;
    }

    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);

    xml.writeStartDocument();
    xml.writeStartElement("view");

    foreach(DataGraph *graph, this->graphs)
    {
        xml.writeStartElement("graph");
        xml.writeAttribute("id", graph->GetID());
        xml.writeAttribute("name", graph->GetName());

        QMap<QString,QColor> plottable = graph->GetlottableData();
        QMap<QString,QColor>::iterator iterator = plottable.begin();

        while(iterator != plottable.end())
        {
            xml.writeEmptyElement("trackable");
            xml.writeAttribute("id", iterator.key());
            xml.writeAttribute("r", QString::number(iterator.value().red()));
            xml.writeAttribute("g", QString::number(iterator.value().green()));
            xml.writeAttribute("b", QString::number(iterator.value().blue()));
        }

        xml.writeEmptyElement("layout");
        xml.writeAttribute("height", QString::number(graph->Layout.Height));
        xml.writeAttribute("width", QString::number(graph->Layout.Width));
        xml.writeAttribute("x", QString::number(graph->Layout.X));
        xml.writeAttribute("y", QString::number(graph->Layout.Y));
        xml.writeAttribute("policies", QString::number(graph->Layout.Policies));
    }
}

void ProjectView::OpenView()
{
    QDir dir(project->Location);
    QFile file(dir.absoluteFilePath(project->Name + QString(".apv")));

    if(file.exists())
    {
        if(file.open(QFile::ReadOnly))
        {
            QXmlStreamReader xml;
            xml.addData(file.readAll());
            while (!xml.atEnd())
            {
                xml.readNext();

                if (xml.isStartElement())
                {
                    if(xml.name() == "graph")
                    {
                        this->graphs.append
                        (
                            new DateGraph
                            (
                                xml.attributes().value("name").toString(),
                                this->project,
                                xml.attributes().value("id").toString()
                            )
                        );
                    }
                    else if(xml.name() == "trackable")
                    {
                        int graphIndex = this->graphs.count() - 1;

                        if(graphIndex < 0)
                            continue;

                        this->graphs.at(graphIndex)->AddPlottable
                        (
                            xml.attributes().value("id").toString(),
                            QColor::fromRgb
                            (
                                xml.attributes().value("r").toInt(),
                                xml.attributes().value("g").toInt(),
                                xml.attributes().value("b").toInt()
                            )
                        );
                    }
                    else if(xml.name() == "layout")
                    {
                        int graphIndex = this->graphs.count() - 1;

                        if(graphIndex < 0)
                            continue;

                        this->graphs.at(graphIndex)->Layout = LayoutData
                        {
                            xml.attributes().value("height").toDouble(),
                            xml.attributes().value("width").toDouble(),
                            xml.attributes().value("x").toUInt(),
                            xml.attributes().value("y").toUInt(),
                            (LayoutData::Policy)xml.attributes().value("policies").toUInt()
                        };
                    }
                }
            }
            if (xml.error())
            {
                qWarning() << "XML ERROR:" << xml.lineNumber() << ": " << xml.errorString();
            }
        }
    }
}

void ProjectView::on_applyRangeButton_clicked()
{
    foreach(DateGraph *graph, this->graphs)
        graph->SetRange(ui->fromTimeEdit->dateTime(), ui->toTimeEdit->dateTime());
}

void ProjectView::on_addEntryButton_clicked()
{
    this->entryDialog = new DataEntryDialog(this->project, this);
    this->connect(this->entryDialog, SIGNAL(UserFinished(bool)), SLOT(UserFinishedEntry(bool)));
    this->entryDialog->show();
}

void ProjectView::UserFinishedEntry(bool newData)
{
    this->entryDialog->hide();
    this->disconnect(this->entryDialog, SIGNAL(UserFinished(bool)), this, SLOT(UserFinishedEntry(bool)));
    delete this->entryDialog;

    if(newData)
        this->ReplotGraphs();
}
