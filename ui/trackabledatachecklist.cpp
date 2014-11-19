#include "trackabledatachecklist.h"

TrackableDataChecklist::TrackableDataChecklist(QObject *parent) :
    QTableWidget(parent)
{
    menu = new Menu(this);

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

        menu->ClearItems();

        MenuItem *newItem = new MenuItem("Add New", item->ContextID);
        this->connect(newItem, SIGNAL(Invoked(QVariant)), SLOT(onAddNewRequest(QVariant)));

        MenuItem *editItem = new MenuItem(tr("Edit"), item->ContextID);
        this->connect(editItem, SIGNAL(Invoked(QVariant)), SLOT(onEditRequest(QVariant)));

        MenuItem *removeItem = new MenuItem("Remove", item->ContextID);
        this->connect(newItem, SIGNAL(Invoked(QVariant)), SLOT(onRemoveRequest(QVariant)));

        MenuItem *trackableItem;

        if(this->trackableData.contains(item->ContextID))
            trackableItem = new MenuItem("Don't Track Data", item->ContextID);
        else
            trackableItem = new MenuItem("Track Data", item->ContextID);

        this->connect(trackableItem, SIGNAL(Invoked(QVariant)), SLOT(onTrackableToggle(QVariant)));

        menu->AddItem(newItem);
        menu->AddItem(editItem);
        menu->AddItem(removeItem);
        menu->AddSeparator();
        menu->AddItem(trackableItem);

        menu->Popup(event->globalPos());

    }
    else
        QTableWidget::mousePressEvent(event);
}

void TrackableDataChecklist::onTrackableToggle(QVariant data)
{
    if(this->trackableData.contains(data.toString()))
        this->trackableData.removeAll(data.toString());
    else
        this->trackableData.append(data.toString());

    this->ResetList();
}

void TrackableDataChecklist::onAddNewRequest(QVariant data)
{
    Q_UNUSED(data);

    if(this->editor != NULL)
        return;

    this->editor = new DataDescriptorEditor();
    this->connect(this->editor, SIGNAL(Canceled()), SLOT(onCancelEdit()));
    this->connect(this->editor, SIGNAL(EditedDescriptor(DataEntry::Descriptor)), SLOT(onEdit(DataEntry::Descriptor)));
    this->connect(this->editor, SIGNAL(NewDescriptor(DataEntry::Descriptor)), SLOT(onAddNew(DataEntry::Descriptor)));
    this->editor->show();
}

void TrackableDataChecklist::onEditRequest(QVariant data)
{
    if(this->editor != NULL)
        return;

    QList<DataEntry::Descriptor> list = ProjectManager::GetDescriptors();

    foreach(DataEntry::Descriptor descriptor, list)
    {
        if(descriptor.ID == data.toString())
        {
            this->editor = new DataDescriptorEditor(descriptor);
            this->connect(this->editor, SIGNAL(Canceled()), SLOT(onCancelEdit()));
            this->connect(this->editor, SIGNAL(EditedDescriptor(DataEntry::Descriptor)), SLOT(onEdit(DataEntry::Descriptor)));
            this->connect(this->editor, SIGNAL(NewDescriptor(DataEntry::Descriptor)), SLOT(onAddNew(DataEntry::Descriptor)));
            this->editor->show();
        }
    }
}

void TrackableDataChecklist::onRemoveRequest(QVariant data)
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
