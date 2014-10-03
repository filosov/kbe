#ifndef OUTPUTWINDOW_H
#define OUTPUTWINDOW_H

#include <QWidget>

class QTextEdit;
class QPushButton;

class OutputWindow : public QWidget
{
    Q_OBJECT
public:
    explicit OutputWindow(QWidget *parent = 0);
    ~OutputWindow();
    void addMessage(const QString& message);
private:
    QTextEdit *mOutputLog;
signals:
    
public slots:
    void clearOutput();
};

#endif // OUTPUTWINDOW_H
