#include "rssentry.h"
#include "ui_rssentry.h"

RSSEntry::RSSEntry(RSSChannel::Item &item, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RSSEntry)
{
    ui->setupUi(this);
    this->setLayout(ui->mainLayout);
}

RSSEntry::~RSSEntry()
{
    delete ui;
}

void RSSEntry::ImageResolved(QImage)
{

}
