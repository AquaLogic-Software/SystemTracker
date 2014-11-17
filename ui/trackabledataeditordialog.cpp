#include "trackabledataeditordialog.h"
#include "ui_trackabledataeditordialog.h"

TrackableDataEditorDialog::TrackableDataEditorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TrackableDataEditorDialog)
{
    ui->setupUi(this);
}

TrackableDataEditorDialog::~TrackableDataEditorDialog()
{
    delete ui;
}
