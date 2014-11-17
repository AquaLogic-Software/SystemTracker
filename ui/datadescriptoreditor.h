#ifndef DATADESCRIPTOREDITOR_H
#define DATADESCRIPTOREDITOR_H

#include <QDialog>
#include <QAbstractButton>
#include "../dataentry.h"

namespace Ui {
class DataDescriptorEditor;
}

class DataDescriptorEditor : public QDialog
{
    Q_OBJECT

public:
    DataDescriptorEditor(QWidget *parent = 0);
    DataDescriptorEditor(DataEntry::Descriptor descriptor, QWidget *parent = 0);
    ~DataDescriptorEditor();

signals:
    void NewDescriptor(DataEntry::Descriptor);
    void EditedDescriptor(DataEntry::Descriptor);
    void Canceled();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::DataDescriptorEditor *ui;
    bool newDescriptor;
    DataEntry::Descriptor descriptor;
};

#endif // DATADESCRIPTOREDITOR_H
