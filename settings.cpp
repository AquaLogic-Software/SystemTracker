#include "settings.h"

QList<AbstractEntry> SettingsManager::globalSettings;

void SettingsManager::AddGlobal(AbstractEntry entry)
{
    for(int i = 0; i < globalSettings.count(); i++)
    {
        if(globalSettings.at(i).GetName() == entry.GetName())
        {
            globalSettings.removeAt(i);
            break;
        }
    }

    globalSettings.append(entry);
}

QList<AbstractEntry> SettingsManager::GetGroupGlobal(QString group)
{
    QList<AbstractEntry> retList;
    retList.clear();

    foreach(AbstractEntry entry, globalSettings)
    {
        if(entry.GetName().startsWith(group))
            retList.append(entry);
    }

    return retList;
}

void SettingsManager::Add(AbstractEntry entry)
{
    for(int i = 0; i < settings.count(); i++)
    {
        if(settings.at(i).GetName() == entry.GetName())
        {
            settings.removeAt(i);
            break;
        }
    }

    settings.append(entry);
}

AbstractEntry SettingsManager::Get(QString name)
{
    foreach(AbstractEntry index, settings)
    {
        if(index.GetName() == name)
        {
            return index;
        }
    }

    return AbstractEntry();
}

QList<AbstractEntry> SettingsManager::GetGroup(QString group)
{
    QList<AbstractEntry> retList;
    retList.clear();

    foreach(AbstractEntry entry, this->settings)
    {
        if(entry.GetName().startsWith(group))
            retList.append(entry);
    }

    return retList;
}

SettingsManager::SettingsManager()
{
}
