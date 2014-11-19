#include "menu.h"

Menu::Menu(QObject *parent)
{
    this->contextMenu = new QMenu(parent);
    this->contextMenu->hide();
    this->contextMenu->clear();
    this->connect(this->contextMenu, SIGNAL(triggered(QAction*)), SLOT(onItemClicked(QAction*)));
}

Menu::~Menu()
{
    this->contextMenu->clear();
}

void Menu::AddItem(MenuItem *item)
{
    QAction *itemAction = new QAction(item->Text, this->contextMenu);
    this->itemMap.insert(itemAction, item);
    this->contextMenu->addAction(itemAction);
}

void Menu::ClearItems()
{
    this->contextMenu->clear();
    this->itemMap.clear();
}

void Menu::Popup(QPoint pos)
{
    this->contextMenu->show();
    this->contextMenu->popup(pos);
}

void Menu::AddSeparator()
{
    this->contextMenu->addSeparator();
}

void Menu::onItemClicked(QAction *action)
{
    QMap<QAction*,MenuItem*>::iterator iterator = this->itemMap.begin();
    while(iterator != this->itemMap.end())
    {
        if(iterator.key() == action)
        {
            (iterator.value())->Invoke();
            return;
        }

        iterator++;
    }
}

MenuItem::MenuItem(QString text, QVariant contextData) : QObject()
{
    this->Text = text;
    this->ContextData = contextData;
}

void MenuItem::Invoke()
{
    emit this->Invoked(this->ContextData);
}
