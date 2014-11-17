#ifndef SETTINGS_H
#define SETTINGS_H

#include <QByteArray>
#include <QList>
#include <QIODevice>
#include <QDataStream>
#include <QBuffer>
#include <QDebug>

#include "abstractentry.h"

template<typename T>
class SettingsEntry : public AbstractEntry
{
private:
    void Serialize(T value)
    {
        QBuffer buffer(&val);
        QDataStream stream(&buffer);
        buffer.open(QIODevice::WriteOnly);

        try
        {
            stream << value;
        }
        catch (...)
        {
            qDebug("Could not add data to the stream");
        }
    }

    T Deserialize()
    {
        QBuffer buffer(&val);
        QDataStream stream(&buffer);
        buffer.open(QIODevice::ReadOnly);

        T temp;

        try
        {
            stream >> temp;
        }
        catch (...)
        {
            qDebug("Could not add data to the stream");
        }

        return temp;
    }

public:

    SettingsEntry(QString name, T value)
    {
        Name = name;
        Serialize(value);
    }

    SettingsEntry(AbstractEntry entry)
    {
        this->Name = entry.GetName();
        this->val.clear();
        this->val.append(entry.GetRawValue());
    }

    SettingsEntry()
    {

    }

    T GetValue()
    {
        return Deserialize();
    }

//    SettingsEntry<T>& operator=(AbstractEntry a)
//    {
//        this->Name = a.GetName();
//        this->val.clear();
//        this->val.append(a.GetRawValue());

//        return this;
//    }

};

class SettingsManager : public QObject
{
    Q_OBJECT
public:

    static void AddGlobal(AbstractEntry entry);
    static AbstractEntry GetGlobal(QString name);
    static QList<AbstractEntry> GetGroupGlobal(QString group);

    void Add(AbstractEntry entry);
    AbstractEntry Get(QString name);
    QList<AbstractEntry> GetGroup(QString group);

    ~SettingsManager()
    {
    }

    SettingsManager();

private:
    QList<AbstractEntry> settings;
    static QList<AbstractEntry> globalSettings;
};

#endif // SETTINGS_H
