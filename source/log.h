#ifndef LOG_H
#define LOG_H

#include <QDialog>
#include <QColor>
#include <mutex>
#include <QList>

namespace Ui {
class log;
}

class Log : public QDialog
{
    Q_OBJECT

public:

    void showLog(QString action = "");

    explicit Log(QWidget *parent = nullptr);
    ~Log();

public slots:

    void addMessage(QString message,int type = 0);
    void addProgress(int percent);
    void showResult();

private:
    Ui::log *ui;
    QList<QString> m_listErrors;
    void showErrors();

private slots:
    void on_CancelButton_clicked();
    void on_BackUpButton_clicked();

 signals:
    void needBackUp();
    void needClearBackUpFolder();
};

#endif // LOG_H
