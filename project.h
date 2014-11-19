#ifndef PROJECT_H
#define PROJECT_H

#include <QString>
#include <QList>
#include <QDate>
#include <QMap>
#include <QDir>
#include <QFile>
#include <QApplication>
#include <QtXml/QtXml>
#include <QUuid>
#include <QMap>
#include <QRgb>

#include "settings.h"
#include "dataentry.h"

class Project
{
public:

    class EntryList
    {
    public:
        DataEntry::Descriptor Descriptor;

        EntryList(QString name, DataEntry::DataType type, quint16 flags, QString id = QUuid::createUuid().toString()) :
            Descriptor(name, type, id)
        {

            this->DataList.clear();
            this->Descriptor.Flags = flags;
        }

        EntryList(DataEntry::Descriptor descriptor)
        {
            this->DataList.clear();
            this->Descriptor = descriptor;
        }

        void Add(DataEntry entry)
        {
            this->DataList.append(entry);
        }

        const DataEntry GetAt(quint32 index)
        {
            return this->DataList.at(index);
        }

        QList<DataEntry> GetRange(QDate from, QDate to)
        {
            QList<DataEntry> returnList;
            returnList.clear();

            foreach(DataEntry entry, this->DataList)
            {
                QDate date = entry.Timestamp.date();

                if((date > from) && (date <= to))
                    returnList.append(entry);
            }

            return returnList;
        }

        QList<DataEntry> DataList;

        EntryList& operator=(const EntryList &a)
        {
            this->Descriptor = DataEntry::Descriptor(a.Descriptor.Name, a.Descriptor.Type, a.Descriptor.ID);
            this->DataList.clear();
            this->DataList = a.DataList;
            return *this;
        }
    };

    const QDateTime Created;

    QString Location;
    QString Name;
    QString ID;
    QString Description;

    QList<EntryList> DataLists;
    QList<DataEntry::Descriptor> TrackableDescriptors;
    QList<QString> PictureList;
    QList<QString> Notes;

    QList<DataEntry> LastEntry;

    Project();

    QList<QString> GetTrackableIDs();
};

class ProjectManager
{
public:

    static void OpenProject(Project *project);
    static void SaveProject(Project *project);
    static void SaveProjectList();
    static void Init();
    static QString GenerateUUID();
    static QList<Project*> RecentProjects;

    static QList<DataEntry::Descriptor> GetDescriptors();
    static DataEntry::Descriptor GetDescriptorByID(QString id);
    static void AddDescriptor(DataEntry::Descriptor descriptor);
    static void UpdateDescriptor(DataEntry::Descriptor descriptor);
    static void RemoveDescriptor(DataEntry::Descriptor descriptor);

private:
    ProjectManager();
    ~ProjectManager();

    static void projectToTop(Project *project);
    static QList<DataEntry::Descriptor> defaultDescriptors;
};

#endif // PROJECT_H
