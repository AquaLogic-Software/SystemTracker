#ifndef TRACKABLEDATAEDITORDIALOG_H
#define TRACKABLEDATAEDITORDIALOG_H

#include <QDialog>

namespace Ui {
class TrackableDataEditorDialog;
}

class TrackableDataEditorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TrackableDataEditorDialog(QWidget *parent = 0);
    ~TrackableDataEditorDialog();

private:
    Ui::TrackableDataEditorDialog *ui;
};

#endif // TRACKABLEDATAEDITORDIALOG_H
