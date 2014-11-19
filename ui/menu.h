#ifndef MENU_H
#define MENU_H

#include <QMenu>
#include <QAction>
#include <QList>
#include <QPoint>
#include <QMap>

class MenuItem : public QObject
{
    Q_OBJECT
public:
    MenuItem(QString text, QVariant contextData);
    QString Text;
    QVariant ContextData;

    void Invoke();

signals:
    void Invoked(QVariant);
};

class Menu : public QObject
{
    Q_OBJECT
public:

    explicit Menu(QObject *parent = 0);
    ~Menu();

    void AddItem(MenuItem* item);
    void ClearItems();
    void Popup(QPoint pos);
    void AddSeparator();

public slots:

private slots:
    void onItemClicked(QAction* action);

private:
    QMenu *contextMenu;
    QMap<QAction*,MenuItem*> itemMap;
};

#endif // MENU_H
