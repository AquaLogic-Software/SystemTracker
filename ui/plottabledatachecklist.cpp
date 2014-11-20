#include "plottabledatachecklist.h"

PlottableDataCheckList::PlottableDataCheckList(QObject *parent) :
    QTableWidget(parent)
{
    this->menu = new Menu();
}

PlottableDataCheckList::~PlottableDataCheckList()
{
    this->clear();
    delete this->menu;
}

QList<DataGraph::PlottableData> PlottableDataCheckList::GetPlottable()
{
    return this->plottableData;
}

void PlottableDataCheckList::SetPlottable(QList<DataGraph::PlottableData> plottable)
{
    this->plottableData.clear();
    this->plottableData.append(plottable);
    this->ResetList();
}

void PlottableDataCheckList::SetTrackable(QList<QString> trackable)
{
    this->trackableData.clear();
    this->trackableData.append(trackable);
}

void PlottableDataCheckList::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        event->accept();
        TableItem *item = reinterpret_cast<TableItem*>(this->itemAt(event->pos()));
        this->menu->ClearItems();

        MenuItem *trackableItem = NULL;

        foreach(DataGraph::PlottableData plottable, this->plottableData)
        {
            if(plottable.Descriptor == item->ContextID)
            {
                MenuItem *editItem = new MenuItem(tr("Edit"), item->ContextID);
                this->connect(editItem, SIGNAL(Invoked(QVariant)), SLOT(onEditRequest(QVariant)));
                this->menu->AddItem(editItem);

                trackableItem = new MenuItem(tr("Untrack"), item->ContextID);
                break;
            }
        }

        if(trackableItem == NULL)
            trackableItem = new MenuItem(tr("Track"), item->ContextID);

        this->connect(trackableItem, SIGNAL(Invoked(QVariant)), SLOT(onTrackableToggle(QVariant)));
        this->menu->AddItem(trackableItem);
        this->menu->Popup(event->globalPos());
    }
    else
        QTableWidget::mousePressEvent(event);
}

void PlottableDataCheckList::onTrackableToggle(QVariant data)
{
    foreach(DataGraph::PlottableData plottable, this->plottableData)
    {
        if(plottable.Descriptor.ID == data.toString())
        {
            this->plottableData.removeOne(plottable);
            this->ResetList();
            return;
        }
    }

    this->plottableData.append
    (
        DataGraph::PlottableData
        {
            ProjectManager::GetDescriptorByID(data.toString()),
            QColorDialog::getColor(QColor(0,0,0), 0, tr("Choose a new color"))
        }
    );

    this->ResetList();
}

void PlottableDataCheckList::onEditRequest(QVariant data)
{
    foreach(DataGraph::PlottableData plottable, this->plottableData)
    {
        if(plottable.Descriptor.ID == data.toString())
        {
            this->plottableData.removeOne(plottable);
            plottable.Color = QColorDialog::getColor(plottable.Color, 0, tr("Choose a new color"));
            this->plottableData.append(plottable);
            this->ResetList();
        }
    }
}

void PlottableDataCheckList::ResetList()
{
    this->clear();

    QList<DataEntry::Descriptor> list = ProjectManager::GetDescriptors();

    this->setRowCount(this->trackableData.count());
    this->setColumnCount(4);

    int row = 0;

    foreach(DataEntry::Descriptor descriptor, list)
    {
        if(!this->trackableData.contains(descriptor.ID))
            continue;

        TableItem *itemName = new TableItem(descriptor.ID, row, 0);
        itemName->setText(descriptor.Name);
        TableItem *itemType = new TableItem(descriptor.ID, row, 1);
        itemType->setText(DataEntry::Descriptor::GetTypeText(descriptor));
        TableItem *itemColor = new TableItem(descriptor.ID, row, 2);
        TableItem *itemTrackable = new TableItem(descriptor.ID, row, 3);
        itemTrackable->setText("No");

        foreach(DataGraph::PlottableData plottable, this->plottableData)
        {
            if(plottable.Descriptor.ID == descriptor.ID)
            {
                itemTrackable->setText("Yes");
                itemColor->setBackgroundColor(plottable.Color);
            }
        }

        this->setItem(row, 0, itemName);
        this->setItem(row, 1, itemType);
        this->setItem(row, 2, itemColor);
        this->setItem(row, 3, itemTrackable);

        row++;
    }

    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->setHorizontalHeaderLabels(QStringList{ tr("Name"), tr("Type"), tr("Color"), tr("Tracking") });
}

PlottableDataCheckList::TableItem::TableItem(QString id, int row, int col) : QTableWidgetItem()
{
    this->ContextID = id;
    this->Location = QPair<int,int>(row,col);
}
