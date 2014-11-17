#ifndef PROJECTENTRY_H
#define PROJECTENTRY_H

#include <QWidget>
#include "../project.h"

namespace Ui {
class ProjectEntry;
}

class ProjectEntry : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectEntry(Project *project,QWidget *parent = 0);
    ~ProjectEntry();

signals:
    void ProjectOpenRequest(Project*);
    void ProjectDeleteRequest(Project*);

private slots:
    void on_pushButton_3_clicked();
    void on_deleteProjectButton_clicked();

private:
    Ui::ProjectEntry *ui;
    Project *project;
};

#endif // PROJECTENTRY_H
