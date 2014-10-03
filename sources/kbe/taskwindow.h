#ifndef TASKWINDOW_H
#define TASKWINDOW_H

#include <QWidget>
#include <QMap>

class QListWidget;
class QListWidgetItem;
class QPushButton;
class QComboBox;
class QLabel;

class Milestone;
class Issue;

class TaskView;

class TaskWindow : public QWidget
{
    Q_OBJECT
public:
    explicit TaskWindow(QWidget *parent = 0);
    ~TaskWindow();

private slots:
    void openViewWindow();
    void openViewWindow(QListWidgetItem *item);
    void milestonesChanged(QList<Milestone*>& list, bool success);
    void selectedMilestoneChanged(int index);
    void issuesChanged(QList<Issue*>& list, bool success);
private:
    QLabel *mProjectNameLabel;
    QComboBox *mMilestoneComboBox;
    QListWidget *mTaskList;
    QPushButton *mViewButton;

    TaskView *mTaskView;

    //! TODO: replace after db usage integration
    QList<Milestone*> mMilestones;
    QList<Issue*> mIssues;
    QMap<QListWidgetItem*, Issue*> mIssueMapping;
    friend class TaskManager;
};

#endif // TASKWINDOW_H
