#include "dataentrydialog.h"
#include "ui_dataentrydialog.h"

DataEntryDialog::DataEntryDialog(Project *project, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataEntryDialog)
{
    ui->setupUi(this);

    this->entryTable = new QTableWidget(this);
    this->entryTable->clear();
    this->ui->mainLayout->insertWidget(1, this->entryTable);
    this->setLayout(ui->mainLayout);

    this->project = project;
    this->entryTable->setRowCount(this->project->TrackableDescriptors.count());
    this->entryTable->setColumnCount(3);

    int row = 0;

    foreach(DataEntry::Descriptor descriptor, this->project->TrackableDescriptors)
    {
        TableItem *itemName = new TableItem(descriptor.ID, row, 0);
        itemName->setText(descriptor.Name);
        TableItem *itemMeasurement = new TableItem(descriptor.ID, row, 0);
        itemMeasurement->setText(DataEntry::Descriptor::GetTypeText(descriptor));
        QWidget *itemEntry = NULL;

        if(descriptor.IsNumerical())
            itemEntry = new QDoubleSpinBox();
        else
            itemEntry = new QLineEdit();

        this->entryTable->setItem(row, 0, itemName);
        this->entryTable->setCellWidget(row, 1, itemEntry);
        this->entryTable->setItem(row, 2, itemMeasurement);

        row++;
    }

    this->entryTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->entryTable->setHorizontalHeaderLabels(QStringList{ QString("Name"), QString("Entry"), QString("Measurement") });
}

DataEntryDialog::~DataEntryDialog()
{
    delete ui;
    delete this->entryTable;
}

void DataEntryDialog::closeEvent(QCloseEvent *event)
{
    emit UserFinished(false);
}

void DataEntryDialog::on_buttonBox_accepted()
{
    for(int i = 0, rows = this->entryTable->rowCount(); i < rows; i++)
    {
        const TableItem *itemName = (TableItem*)this->entryTable->item(i, 0);

        //Again slow..needs to be replaced
        for(int i = 0; i < this->project->DataLists.count(); i++)
        {
            if(this->project->DataLists.at(i).Descriptor.ID == itemName->ContextID)
            {
                DataEntry entry;
                entry.ListDescriptor = &this->project->DataLists.at(i).Descriptor;

                if(this->project->DataLists.at(i).Descriptor.IsNumerical())
                {
                    QDoubleSpinBox *box = reinterpret_cast<QDoubleSpinBox*>(this->entryTable->cellWidget(i, 1));
                    entry.SetValue(box->value());
                }
                else
                {
                    QLineEdit *edit = reinterpret_cast<QLineEdit*>(this->entryTable->cellWidget(i, 1));
                    entry.SetValue(edit->text());
                }

                this->project->DataLists.at(i).Add(entry);
                break;
            }
        }
//        foreach(Project::EntryList list, this->project->DataLists)
//        {
//            if(list.Descriptor.ID == itemName->ContextID)
//            {
//                DataEntry entry;
//                entry.ListDescriptor = &list.Descriptor;

//                if(list.Descriptor.IsNumerical())
//                {
//                    QDoubleSpinBox *box = reinterpret_cast<QDoubleSpinBox*>(this->entryTable->cellWidget(i, 1));
//                    entry.SetValue(box->value());
//                }
//                else
//                {
//                    QLineEdit *edit = reinterpret_cast<QLineEdit*>(this->entryTable->cellWidget(i, 1));
//                    entry.SetValue(edit->text());
//                }

//                list.Add(entry);
//                break;
//            }
//        }
    }

    emit UserFinished(true);
}

void DataEntryDialog::on_buttonBox_rejected()
{
    emit UserFinished(false);
}

DataEntryDialog::TableItem::TableItem(QString id, int row, int col) : QTableWidgetItem()
{
    this->ContextID = id;
    this->Location = QPair<int,int>(row,col);
}
