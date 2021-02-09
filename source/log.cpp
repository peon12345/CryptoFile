#include "log.h"
#include "ui_log.h"

Log::Log(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::log)
{
    ui->setupUi(this);
}

Log::~Log()
{
    delete ui;
}

void Log::showLog(QString action){

    emit needClearBackUpFolder();

    m_listErrors.clear();
    ui->TextArea->clear();
    ui->progressBar->setValue(1);
    ui->CancelButton->setEnabled(false);
    ui->BackUpButton->hide();
    ui->labelAction->setText(action);
    this->show();
}

void Log::on_CancelButton_clicked()
{
    emit needClearBackUpFolder();
    this->close();
}

void Log::addMessage(QString message,int type){
    switch (type) {
    case 1:
        ui->TextArea->setTextColor(QColor("green"));
        break;

    case 2:
        ui->TextArea->setTextColor(QColor("orange"));
        break;
    case 3:
        ui->TextArea->setTextColor(QColor("red"));
        m_listErrors.push_back(message);
        break;

    default:
        ui->TextArea->setTextColor(QColor("black"));
        break;
    }
    ui->TextArea->append(message);
}


void Log::showResult(){
    showErrors();
    ui->progressBar->setValue(100);
    ui->CancelButton->setText("Выход");
    ui->CancelButton->setEnabled(true);
}

void Log::showErrors(){
    if(!m_listErrors.empty()){

        ui->CancelButton->setText("Выход");
        ui->BackUpButton->show();

        ui->TextArea->setTextColor(QColor("red"));
        ui->TextArea->append("Ошибки:");

        QList<QString>::const_iterator it;
        for (it = m_listErrors.begin(); it != m_listErrors.end(); ++it)
            ui->TextArea->append(*it);
    }
}

void Log::addProgress(int percent){

    static std::mutex mProgress;

    if(ui->progressBar->value() >= 100){
        return void();
    }
    mProgress.lock();

    int value = ui->progressBar->value() + percent;

    if(value >= 100){
        value = 100;
    }

    ui->progressBar->setValue(value);
    mProgress.unlock();
}

void Log::on_BackUpButton_clicked()
{
    emit needBackUp();
}
