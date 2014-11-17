#ifndef RSSCHANNEL_H
#define RSSCHANNEL_H

#include <QString>

class RSSChannel
{
public:

    class Item
    {
    public:
        const QString Title;
        const QString Description;
        const QString Link;
        const QString ImageURL;

        Item(QString title, QString description, QString link, QString imageURL = "")
            : Title(title), Description(description), Link(link), ImageURL(imageURL) { }
    };

    QString Title;
    QString Description;
    QString Link;
    QString FeedURL;

    RSSChannel(QString title, QString description, QString link, QString feedURL)
        : Title(title), Description(description), Link(link), FeedURL(feedURL) { }
};

#endif // RSSCHANNEL_H
