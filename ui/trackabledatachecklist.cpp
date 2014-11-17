#include "trackabledatachecklist.h"

TrackableDataChecklist::TrackableDataChecklist(QObject *parent) :
    QTableWidget(parent)
{
    menu = new Menu(this);
    menu->hide();
    this->connect(this->menu, SIGNAL(AddNew()), SLOT(onAddNewRequest()));
    this->connect(this->menu, SIGNAL(Edit(QString)), SLOT(onEditRequest(QString)));
    this->connect(this->menu, SIGNAL(TrackableToggle(QString)), SLOT(onTrackableToggle(QString)));
    this->connect(this->menu, SIGNAL(Remove(QString)), SLOT(onRemoveRequest(QString)));
    this->editor = NULL;
    this->project = NULL;
    this->ResetList();
}

TrackableDataChecklist::TrackableDataChecklist(Project *project, QObject *parent)
{
    this->project = project;
}

TrackableDataChecklist::~TrackableDataChecklist()
{
    this->clear();
    delete this->menu;
}

void TrackableDataChecklist::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        event->accept();
        TableItem *item = reinterpret_cast<TableItem*>(this->itemAt(event->pos()));

        if(item == NULL)
            return;

        menu->SetContextID(item->ContextID);
        menu->SetTrackable(this->trackableData.contains(item->ContextID));
        menu->popup(event->globalPos());
    }
    else
        QTableWidget::mousePressEvent(event);
}

void TrackableDataChecklist::onTrackableToggle(QString id)
{
    if(this->trackableData.contains(id))
        this->trackableData.removeAll(id);
    else
        this->trackableData.append(id);

    this->ResetList();
}

void TrackableDataChecklist::onAddNewRequest()
{
    if(this->editor != NULL)
        return;

    this->editor = new DataDescriptorEditor();
    this->connect(this->editor, SIGNAL(Canceled()), SLOT(onCancelEdit()));
    this->connect(this->editor, SIGNAL(EditedDescriptor(DataEntry::Descriptor)), SLOT(onEdit(DataEntry::Descriptor)));
    this->connect(this->editor, SIGNAL(NewDescriptor(DataEntry::Descriptor)), SLOT(onAddNew(DataEntry::Descriptor)));
    this->editor->show();
}

void TrackableDataChecklist::onEditRequest(QString id)
{
    if(this->editor != NULL)
        return;
    QList<DataEntry::Descriptor> list = ProjectManager::GetDescriptors();

    foreach(DataEntry::Descriptor descriptor, list)
    {
        if(descriptor.ID == id)
        {
            this->editor = new DataDescriptorEditor(descriptor);
            this->connect(this->editor, SIGNAL(Canceled()), SLOT(onCancelEdit()));
            this->connect(this->editor, SIGNAL(EditedDescriptor(DataEntry::Descriptor)), SLOT(onEdit(DataEntry::Descriptor)));
            this->connect(this->editor, SIGNAL(NewDescriptor(DataEntry::Descriptor)), SLOT(onAddNew(DataEntry::Descriptor)));
            this->editor->show();
        }
    }
}

void TrackableDataChecklist::onRemoveRequest(QString id)
{

}

void TrackableDataChecklist::onAddNew(DataEntry::Descriptor descriptor)
{
    ProjectManager::AddDescriptor(descriptor);
    this->disconnect(this->editor, SIGNAL(Canceled()));
    this->disconnect(this->editor, SIGNAL(EditedDescriptor(DataEntry::Descriptor)));
    this->disconnect(this->editor, SIGNAL(NewDescriptor(DataEntry::Descriptor)));
    this->editor->hide();

    delete this->editor;
    this->editor = NULL;
}

void TrackableDataChecklist::onEdit(DataEntry::Descriptor descriptor)
{
    ProjectManager::UpdateDescriptor(descriptor);
    this->disconnect(this->editor, SIGNAL(Canceled()));
    this->disconnect(this->editor, SIGNAL(EditedDescriptor(DataEntry::Descriptor)));
    this->disconnect(this->editor, SIGNAL(NewDescriptor(DataEntry::Descriptor)));
    this->editor->hide();

    delete this->editor;
    this->editor = NULL;
}

void TrackableDataChecklist::onCancelEdit()
{
    this->disconnect(this->editor, SIGNAL(Canceled()));
    this->disconnect(this->editor, SIGNAL(EditedDescriptor(DataEntry::Descriptor)));
    this->disconnect(this->editor, SIGNAL(NewDescriptor(DataEntry::Descriptor)));
    this->editor->hide();

    delete this->editor;
    this->editor = NULL;
}

void TrackableDataChecklist::ResetList()
{
    this->clear();

    QList<DataEntry::Descriptor> list;

    if(this->project == NULL)
        list = ProjectManager::GetDescriptors();
    else
        list = this->project->TrackableDescriptors;

    this->setRowCount(list.count());
    this->setColumnCount(3);

    int row = 0;

    foreach(DataEntry::Descriptor descriptor, list)
    {
        TableItem *itemName = new TableItem(descriptor.ID, row, 0);
        itemName->setText(descriptor.Name);
        TableItem *itemType = new TableItem(descriptor.ID, row, 1);
        itemType->setText(DataEntry::Descriptor::GetTypeText(descriptor));
        TableItem *itemTrackable = new TableItem(descriptor.ID, row, 2);

        if(trackableData.contains(descriptor.ID))
            itemTrackable->setText(QString("Yes"));
        else
            itemTrackable->setText(QString("No"));

        this->setItem(row, 0, itemName);
        this->setItem(row, 1, itemType);
        this->setItem(row, 2, itemTrackable);

        row++;
    }

    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->setHorizontalHeaderLabels(QStringList{ QString("Name"), QString("Type"), QString("Tracking") });
}

Menu::Menu(QObject *parent) : QMenu(parent)
{
    contextID.clear();

    this->editAction = new QAction("Edit", this);
    this->editAction->setCheckable(false);
    this->connect(this->editAction, SIGNAL(triggered(bool)), SLOT(onEditActionTriggered(bool)));

    this->addNewAction = new QAction("Add New", this);
    this->addNewAction->setCheckable(false);
    this->connect(this->addNewAction, SIGNAL(triggered(bool)), SLOT(onAddNewActionTriggered(bool)));

    this->removeAction = new QAction("Remove", this);
    this->removeAction->setCheckable(false);
    this->connect(this->removeAction, SIGNAL(triggered(bool)), SLOT(onRemoveActionTriggered(bool)));

    this->trackableAction = new QAction("Trackable", this);
    this->trackableAction->setCheckable(true);
    this->trackableAction->setChecked(false);
    this->connect(this->trackableAction, SIGNAL(triggered(bool)), SLOT(onTrackableActionTriggered(bool)));

    this->addAction(this->editAction);
    this->addAction(this->addNewAction);
    this->addAction(this->trackableAction);
    this->addAction(this->removeAction);
}

void Menu::SetContextID(QString id)
{
    contextID = id;
}

void Menu::SetTrackable(bool trackable)
{
    this->trackableAction->setChecked(trackable);
}

QList<QString> TrackableDataChecklist::GetTrackableIDs()
{
    return this->trackableData;
}

void TrackableDataChecklist::SetTrackableIDs(QList<QString> ids)
{
    this->trackableData.clear();
    this->ResetList();
}

TrackableDataChecklist::TableItem::TableItem(QString id, int row, int col) : QTableWidgetItem()
{
    this->ContextID = id;
    this->Location = QPair<int,int>(row,col);
}
