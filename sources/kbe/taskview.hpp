#ifndef TASKVIEW_HPP
#define TASKVIEW_HPP

#include <QDialog>

class QTextEdit;
class Issue;

class TaskView : public QDialog
{
    Q_OBJECT
public:
    explicit TaskView(QWidget *parent = 0);
    ~TaskView();
    void setIssue(const Issue* const issue);
private:
    QTextEdit *mView;
};

#endif // TASKVIEW_HPP
