#ifndef RSSENTRY_H
#define RSSENTRY_H

#include <QWidget>
#include "../rss/rsschannel.h"

namespace Ui {
class RSSEntry;
}

class RSSEntry : public QWidget
{
    Q_OBJECT
    RSSChannel::Item *item;
    QPixmap image;
public:
    explicit RSSEntry(RSSChannel::Item &item, QWidget *parent = 0);
    ~RSSEntry();

protected slots:
    void ImageResolved(QImage);
private:
    Ui::RSSEntry *ui;
};

#endif // RSSENTRY_H
