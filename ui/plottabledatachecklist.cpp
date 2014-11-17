#include "plottabledatachecklist.h"

PlottableDataCheckList::PlottableDataCheckList(QObject *parent) :
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

PlottableDataCheckList::PlottableDataCheckList(Project *project, QObject *parent)
{
    this->project = project;
}

PlottableDataCheckList::~PlottableDataCheckList()
{
    this->clear();
    delete this->menu;
}

void PlottableDataCheckList::mousePressEvent(QMouseEvent *event)
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

void PlottableDataCheckList::onTrackableToggle(QString id)
{
    foreach(DataGraph::PlottableData plottable, this->trackableData)
    {
        if(plottable.Descriptor.ID == id)
        {
            this->trackableData.removeOne(plottable);
        }
    }

    this->ResetList();
}

void PlottableDataCheckList::onAddNewRequest()
{
    if(this->editor != NULL)
        return;

    this->colorDialog = new QColorDialog()
    this->connect(this->editor, SIGNAL(Canceled()), SLOT(onCancelEdit()));
    this->connect(this->editor, SIGNAL(EditedDescriptor(DataEntry::Descriptor)), SLOT(onEdit(DataEntry::Descriptor)));
    this->connect(this->editor, SIGNAL(NewDescriptor(DataEntry::Descriptor)), SLOT(onAddNew(DataEntry::Descriptor)));
    this->editor->show();
}

void PlottableDataCheckList::onEditRequest(QString id)
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

void PlottableDataCheckList::onRemoveRequest(QString id)
{

}

void PlottableDataCheckList::onAddNew(DataEntry::Descriptor descriptor)
{
    ProjectManager::AddDescriptor(descriptor);
    this->disconnect(this->colorDialog, SIGNAL(Canceled()));
    this->disconnect(this->colorDialog, SIGNAL(EditedDescriptor(DataEntry::Descriptor)));
    this->disconnect(this->colorDialog, SIGNAL(NewDescriptor(DataEntry::Descriptor)));
    this->colorDialog->hide();

    delete this->colorDialog;
    this->colorDialog = NULL;
}

void PlottableDataCheckList::onEdit(DataEntry::Descriptor descriptor)
{
    this->disconnect(this->colorDialog, SIGNAL(rejected()));
    this->disconnect(this->colorDialog, SIGNAL(accepted()));
    this->colorDialog->hide();

    delete this->colorDialog;
    this->colorDialog = NULL;
}

void PlottableDataCheckList::onCancelEdit()
{
    this->disconnect(this->colorDialog, SIGNAL(rejected()));
    this->disconnect(this->colorDialog, SIGNAL(accepted()));
    this->colorDialog->hide();

    delete this->colorDialog;
    this->colorDialog = NULL;
}

void PlottableDataCheckList::ResetList()
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
