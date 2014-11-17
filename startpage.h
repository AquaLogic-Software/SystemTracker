#ifndef STARTPAGE_H
#define STARTPAGE_H

#include <QWidget>
#include "ui/projectentry.h"
#include "project.h"

namespace Ui {
class StartPage;
}

class StartPage : public QWidget
{
    Q_OBJECT

public:
    explicit StartPage(QWidget *parent = 0);
    ~StartPage();

signals:
    void ProjectOpenRequest(Project*);
    void ProjectDeleteRequest(Project*);

protected slots:
    void requestOpenProject(Project *project);
    void requestDeleteProject(Project *project);

private:
    Ui::StartPage *ui;
};

#endif // STARTPAGE_H
