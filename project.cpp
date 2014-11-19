#include "project.h"

QList<Project*> ProjectManager::RecentProjects;
QList<DataEntry::Descriptor> ProjectManager::defaultDescriptors;

Project::Project()
{
}

QList<QString> Project::GetTrackableIDs()
{
    QList<QString> ids;
    ids.clear();

    foreach(DataEntry::Descriptor descriptor, this->TrackableDescriptors)
        ids.append(descriptor.ID);

    return ids;
}

void ProjectManager::OpenProject(Project *project)
{
    QDir dir(project->Location);
    QFile file(dir.absoluteFilePath(project->Name + QString(".apf")));

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
                    if(xml.name() == "project")
                    {
                        project->Name = xml.attributes().value("name").toString();
                        project->ID = xml.attributes().value("id").toString();
                    }
                    else if(xml.name() == "datalist")
                    {
                        project->DataLists.append
                        (
                            Project::EntryList
                            (
                                xml.attributes().value("name").toString(),
                                xml.attributes().value("type").toUInt(),
                                xml.attributes().value("flags").toUInt(),
                                xml.attributes().value("id").toString()
                            )
                        );
                    }
                    else if(xml.name() == "entry")
                    {
                        DataEntry entry;
                        int i = project->DataLists.count();
                        entry.Timestamp = QDateTime::fromString(xml.attributes().value("timestamp").toString(), Qt::TextDate);
                        entry.SetValue(xml.text().toString());
                        entry.ListDescriptor = &project->DataLists.at(i).Descriptor;

                        project->DataLists.at(i).Add(entry);
                    }
                    else if(xml.name() == "location")
                        project->Location = xml.readElementText();
                }
            }
            if (xml.error())
            {
                qWarning() << "XML ERROR:" << xml.lineNumber() << ": " << xml.errorString();
            }
        }

        projectToTop(project);
        SaveProjectList();
    }
}

void ProjectManager::SaveProject(Project *project)
{
    QDir dir(project->Location);
    QFile file(dir.absoluteFilePath(project->Name + QString(".apf")));

    if(!file.open(QFile::WriteOnly))
    {
        qWarning() << "Could not create file " << file.fileName();
        return;
    }

    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);

    xml.writeStartDocument();
    xml.writeStartElement("project");
    xml.writeAttribute("name", project->Name);
    xml.writeAttribute("created", project->Created.toString(Qt::TextDate));

    foreach(DataEntry::Descriptor descriptor, project->TrackableDescriptors)
        xml.writeTextElement("trackable", descriptor.ID);

    xml.writeTextElement("description",project->Description);
    xml.writeTextElement("location", project->Location);

    foreach(Project::EntryList list, project->DataLists)
    {
        xml.writeStartElement(QString("datalist"));
        xml.writeAttribute(QString("name"), list.Descriptor.Name);
        xml.writeAttribute(QString("id"), list.Descriptor.ID);
        xml.writeAttribute(QString("type"), QString::number(list.Descriptor.Type));
        xml.writeAttribute(QString("flags"), QString::number(list.Descriptor.Flags));

        foreach(DataEntry entry, list.DataList)
        {
            xml.writeTextElement(QString("entry"), entry.GetString());
            xml.writeAttribute(QString("timestamp"), entry.Timestamp.toString(Qt::TextDate));
        }

        xml.writeEndElement();
    }

    foreach(QString note, project->Notes)
        xml.writeTextElement("note", note);

    xml.writeEndElement();
    file.close();

    projectToTop(project);
    SaveProjectList();
}

void ProjectManager::SaveProjectList()
{
    QDir dir(QApplication::applicationDirPath());
    QFile file(dir.absoluteFilePath("projects.apl"));

    if(!file.open(QFile::WriteOnly))
    {
        qWarning() << "Could not create file " << file.fileName();
        return;
    }

    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("settings");

    foreach(DataEntry::Descriptor descriptor, defaultDescriptors)
    {
        xml.writeEmptyElement("descriptor");
        xml.writeAttribute("name", descriptor.Name);
        xml.writeAttribute("id", descriptor.ID);
        xml.writeAttribute("flags", descriptor.Flags);
    }

    foreach(Project *project, RecentProjects)
    {
        xml.writeStartElement("project");
        xml.writeAttribute("name", project->Name);
        xml.writeAttribute("id", project->ID);
        xml.writeAttribute("created", QDateTime::currentDateTime().toString());
        xml.writeTextElement("description",project->Description);
        xml.writeTextElement("location", project->Location);

        foreach(DataEntry entry, project->LastEntry)
        {
            xml.writeEmptyElement("lastentry");
            xml.writeAttribute("id", entry.ListDescriptor->ID);
            xml.writeAttribute("value", entry.GetString());
        }

        xml.writeEndElement();
    }

    xml.writeEndElement();
    xml.writeEndDocument();
    file.close();
}

void ProjectManager::Init()
{
    RecentProjects.clear();
    defaultDescriptors.clear();

    QDir dir(QApplication::applicationDirPath());
    QFile file(dir.absoluteFilePath(QString("projects.apl")));

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
                    if (xml.name() == "project")
                    {
                        RecentProjects.append(new Project());
                        RecentProjects.at(RecentProjects.count() - 1)->Name = xml.attributes().value("name").toString();
                        RecentProjects.at(RecentProjects.count() - 1)->ID = xml.attributes().value("id").toString();
                        RecentProjects.at(RecentProjects.count() - 1)->Created = QDateTime::fromString(xml.attributes().value("created").toString());
                    }
                    else if(xml.name() == "descriptor")
                    {
                        defaultDescriptors.append
                        (
                            DataEntry::Descriptor
                            (
                                xml.attributes().value("name").toString(),
                                xml.attributes().value("type").toUInt(),
                                xml.attributes().value("id").toString(),
                                xml.attributes().value("flags").toUInt()
                            )
                        );
                    }
                    else if(xml.name() == "description")
                    {
                        if(RecentProjects.count() == 0)
                            continue;

                        RecentProjects.at(RecentProjects.count() - 1)->Description = xml.text().toString();
                    }
                    else if(xml.name() == "location")
                    {
                        if(RecentProjects.count() == 0)
                            continue;
                        QString location = xml.readElementText();
                        RecentProjects.at(RecentProjects.count() - 1)->Location = location;
                    }
                }
            }
            if (xml.error())
            {
                qWarning() << "XML ERROR:" << xml.lineNumber() << ": " << xml.errorString();
            }

            file.close();
        }
    }

    //Default fun
    else
    {
        defaultDescriptors.append
        (
            DataEntry::Descriptor
            (
                QString("pH"),
                DataEntry::Type_Numerical | DataEntry::Type_PH,
                QUuid::createUuid().toString()
            )
        );

        defaultDescriptors.append
        (
            DataEntry::Descriptor
            (
                QString("kH"),
                DataEntry::Type_Numerical | DataEntry::Type_PPM,
                QUuid::createUuid().toString()
            )
        );

        defaultDescriptors.append
        (
            DataEntry::Descriptor
            (
                QString("gH"),
                DataEntry::Type_Numerical | DataEntry::Type_PPM,
                QUuid::createUuid().toString()
            )
        );

        defaultDescriptors.append
        (
            DataEntry::Descriptor
            (
                QString("Calcium"),
                DataEntry::Type_Numerical | DataEntry::Type_PPM,
                QUuid::createUuid().toString()
            )
        );

        defaultDescriptors.append
        (
            DataEntry::Descriptor
            (
                QString("Potassium"),
                DataEntry::Type_Numerical | DataEntry::Type_PPM,
                QUuid::createUuid().toString()
            )
        );

        defaultDescriptors.append
        (
            DataEntry::Descriptor
            (
                QString("Magnesium"),
                DataEntry::Type_Numerical | DataEntry::Type_PPM,
                QUuid::createUuid().toString()
            )
        );

        defaultDescriptors.append
        (
            DataEntry::Descriptor
            (
                QString("Iron"),
                DataEntry::Type_Numerical | DataEntry::Type_PPM,
                QUuid::createUuid().toString()
            )
        );

        defaultDescriptors.append
        (
            DataEntry::Descriptor
            (
                QString("Feeding"),
                DataEntry::Type_Numerical | DataEntry::Type_G,
                QUuid::createUuid().toString()
            )
        );

        defaultDescriptors.append
        (
            DataEntry::Descriptor
            (
                QString("Chlorine"),
                DataEntry::Type_Numerical | DataEntry::Type_PPM,
                QUuid::createUuid().toString()
            )
        );

        defaultDescriptors.append
        (
            DataEntry::Descriptor
            (
                QString("Copper"),
                DataEntry::Type_Numerical | DataEntry::Type_PPM,
                QUuid::createUuid().toString()
            )
        );

        defaultDescriptors.append
        (
            DataEntry::Descriptor
            (
                QString("Sulphates"),
                DataEntry::Type_Numerical | DataEntry::Type_PPM,
                QUuid::createUuid().toString()
            )
        );

        defaultDescriptors.append
        (
            DataEntry::Descriptor
            (
                QString("Phosphates"),
                DataEntry::Type_Numerical | DataEntry::Type_PPM,
                QUuid::createUuid().toString()
            )
        );

        defaultDescriptors.append
        (
            DataEntry::Descriptor
            (
                QString("Copper"),
                DataEntry::Type_Numerical | DataEntry::Type_PPM,
                QUuid::createUuid().toString()
            )
        );

        defaultDescriptors.append
        (
            DataEntry::Descriptor
            (
                QString("Temperature"),
                DataEntry::Type_Numerical | DataEntry::Type_C,
                QUuid::createUuid().toString()
            )
        );

        defaultDescriptors.append
        (
            DataEntry::Descriptor
            (
                QString("Water Level"),
                DataEntry::Type_Numerical | DataEntry::Type_PPM,
                QUuid::createUuid().toString()
            )
        );
    }
}

void ProjectManager::projectToTop(Project *project)
{
    for(int i = 0; i < RecentProjects.count(); i++)
    {
        if(RecentProjects.at(i)->ID == project->ID)
        {
            RecentProjects.move(i,0);
            return;
        }
    }

    RecentProjects.push_front(project);
}

QString ProjectManager::GenerateUUID()
{
    QString ret;
    bool reissue = true;

    while(reissue)
    {
        ret.clear();
        ret.append(QUuid::createUuid().toString());
        reissue = false;

        foreach(Project *project, RecentProjects)
        {
            if(project->ID == ret)
                reissue = true;
        }
    }

    return ret;
}

QList<DataEntry::Descriptor> ProjectManager::GetDescriptors()
{
    return defaultDescriptors;
}

DataEntry::Descriptor ProjectManager::GetDescriptorByID(QString id)
{
    foreach(DataEntry::Descriptor descriptor, defaultDescriptors)
    {
        if(descriptor.ID == id)
            return descriptor;
    }
}

void ProjectManager::AddDescriptor(DataEntry::Descriptor descriptor)
{
    defaultDescriptors.append(descriptor);
}

void ProjectManager::UpdateDescriptor(DataEntry::Descriptor descriptor)
{
    for(int i = 0; i < defaultDescriptors.count(); i++)
    {
        if(defaultDescriptors.at(i).ID == descriptor.ID)
        {
            defaultDescriptors.removeAt(i);
            defaultDescriptors.append(descriptor);
            break;
        }
    }
}

void ProjectManager::RemoveDescriptor(DataEntry::Descriptor descriptor)
{
    for(int i = 0; i < defaultDescriptors.count(); i++)
    {
        if(defaultDescriptors.at(i).ID == descriptor.ID)
        {
            defaultDescriptors.removeAt(i);
            break;
        }
    }
}
