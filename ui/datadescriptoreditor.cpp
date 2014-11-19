#include "datadescriptoreditor.h"
#include "ui_datadescriptoreditor.h"

DataDescriptorEditor::DataDescriptorEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataDescriptorEditor())
{
    ui->setupUi(this);
    this->setLayout(ui->mainLayout);

    for(int i = 0x0010; i <= 0xFFF0; i += (quint16)(1 << 4))
    {
        DataEntry::Descriptor descriptor("",(DataEntry::DataType)i,"");
        QString text = DataEntry::Descriptor::GetTypeText(descriptor);

        if(text.isNull() || text.isEmpty())
            break;

        this->ui->measurementBox->insertItem(this->ui->measurementBox->count(), text);
    }

    this->newDescriptor = true;
    this->descriptor = DataEntry::Descriptor("",0, QUuid::createUuid().toString());
    this->setWindowTitle(QString("AquaLogic - New Data Descriptor Dialog"));
}

DataDescriptorEditor::DataDescriptorEditor(DataEntry::Descriptor descriptor, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataDescriptorEditor())
{
    ui->setupUi(this);
    this->setLayout(ui->mainLayout);
    ui->measurementBox->clear();

    for(int i = 0x0010; i <= 0xFFF0; i += (quint16)(1 << 4))
    {
        QString text = DataEntry::Descriptor::GetTypeText(DataEntry::Descriptor("",(DataEntry::DataType)i,""));

        if(text.isNull() || text.isEmpty())
            break;

        this->ui->measurementBox->insertItem(this->ui->measurementBox->count(), text);
    }

    this->descriptor = descriptor;
    this->ui->nameEdit->setText(this->descriptor.Name);
    this->ui->dataTypeBox->setCurrentIndex((int)this->descriptor.IsString());
    this->ui->measurementBox->setCurrentIndex((int)((quint16)this->descriptor.Type >> 4) - 1);

    this->newDescriptor = false;
    this->setWindowTitle(QString("AquaLogic - Edit Data Descriptor Dialog"));
}

DataDescriptorEditor::~DataDescriptorEditor()
{
    delete ui;
}

void DataDescriptorEditor::closeEvent(QCloseEvent event)
{
    emit Canceled();
}

void DataDescriptorEditor::on_buttonBox_clicked(QAbstractButton *button)
{
    QDialogButtonBox::StandardButton btn = this->ui->buttonBox->standardButton(button);

    switch(btn)
    {
    case QDialogButtonBox::Reset:
        ui->nameEdit->setText(descriptor.Name);

        if(descriptor.IsNumerical())
            ui->dataTypeBox->setCurrentIndex(0);
        else
            ui->dataTypeBox->setCurrentIndex(1);

        ui->measurementBox->setCurrentIndex((quint32)(descriptor.Type >> 4));
        break;
    case QDialogButtonBox::Apply:
        this->descriptor.Name = this->ui->nameEdit->text();
        this->descriptor.Type = (quint16)(1 << ui->dataTypeBox->currentIndex());
        this->descriptor.Type |= (quint16)(0x0010 + (quint16)(ui->measurementBox->currentIndex() << 4));
        this->descriptor.Flags = 0;

        if(this->newDescriptor)
        {
            this->descriptor.ID = QUuid::createUuid().toString();
            this->newDescriptor = false;
            emit NewDescriptor(this->descriptor);
        }
        else
            emit EditedDescriptor(this->descriptor);

        break;
    case QDialogButtonBox::Cancel:
        emit this->Canceled();
        break;
    }
}
