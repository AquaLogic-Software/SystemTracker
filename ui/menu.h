#ifndef MENU_H
#define MENU_H

#include <QMenu>
#include <QAction>
#include <QList>
#include <QMap>

class Menu : public QMenu
{
    Q_OBJECT

public:
    explicit Menu(QObject *parent = 0);

signals:
    ItemSelected(QAction);

public slots:

private slots:
    void ItemClicked();

};

#endif // MENU_H
