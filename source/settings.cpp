#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settings)
{
    ui->setupUi(this);

    QSettings settings(ORGANIZATION, APPL);

    ui->checkBoxAutoDel->setChecked(settings.value(SETTINGS_AUTO_DEL, false).toBool());
    ui->checkBoxCopyFile->setChecked(settings.value(SETTINGS_ALL_CHECK, false).toBool());
    ui->comboBoxAlg->setCurrentIndex(settings.value(SETTINGS_ALG_ID,0).toInt());

    parametrs.autoDelete = ui->checkBoxAutoDel->isChecked();
    parametrs.copy = ui->checkBoxCopyFile->isChecked();
    parametrs.serialUSB = ui->lineEditSerialUSB->text();
    parametrs.serialHDD = ui->lineEditSerialHDD->text();
    parametrs.algID = ui->comboBoxAlg->currentIndex();
}

Settings::~Settings()
{
    delete ui;
}

Parametrs& Settings::getParams(){
    return parametrs;
}

bool Settings::copy(){
    return ui->checkBoxCopyFile->isChecked();
}

bool Settings::autoDel(){
    return ui->checkBoxAutoDel->isChecked();
}

QString Settings::getSerial(QString deviceType){

    if (deviceType == "USB"){
        if(ui->lineEditSerialUSB->text() != ""){
            return ui->lineEditSerialUSB->text();
        }
    }

    if(deviceType == "HDD"){
        if(ui->lineEditSerialHDD->text() != ""){
            return ui->lineEditSerialHDD->text();
        }
    }

    return "";
}

void Settings::on_pushButtonSave_clicked()
{
    QSettings settings(ORGANIZATION, APPL);

    if(ui->checkBoxAutoDel->isChecked()){
        settings.setValue(SETTINGS_AUTO_DEL, true);
    } else {
        settings.setValue(SETTINGS_AUTO_DEL, false);
    }

    if(ui->checkBoxCopyFile->isChecked()){
        settings.setValue(SETTINGS_ALL_CHECK, true);
    } else {
        settings.setValue(SETTINGS_ALL_CHECK, false);
    }

    settings.setValue(SETTINGS_ALG_ID,ui->comboBoxAlg->currentIndex());

    settings.sync();

    parametrs.autoDelete = ui->checkBoxAutoDel->isChecked();
    parametrs.copy = ui->checkBoxCopyFile->isChecked();
    parametrs.serialUSB = ui->lineEditSerialUSB->text();
    parametrs.serialHDD = ui->lineEditSerialHDD->text();
    parametrs.algID = ui->comboBoxAlg->currentIndex();

    this->close();
}

void Settings::on_pushButtonCancel_clicked()
{
    this->close();
}

void Settings::on_pushButtonAddIO_clicked()
{   

    if(ui->lineEditSerialUSB->text() != ""){
        ui->pushButtonAddIO->setText("Добавить");
        ui->lineEditSerialUSB->setText("");
        return void();
    }

    ui->lineEditSerialUSB->setText(device.getSerialDevice("USB"));

    if(ui->lineEditSerialUSB->text() == ""){

        QMessageBox msgBox;
        msgBox.setWindowIcon(QIcon(QPixmap(":/png/5.png")));
        msgBox.setWindowTitle("Ошибка");
        msgBox.setIconPixmap(QPixmap(":/gifs/resent.gif"));
        msgBox.setText("Не удалось обнаружить устройство");
        msgBox.exec();
    } else {

        ui->pushButtonAddIO->setText("Удалить");
    }


}

void Settings::on_pushButtonAddIO_2_clicked()
{

    if(ui->lineEditSerialHDD->text() != ""){
        ui->pushButtonAddIO_2->setText("Добавить");
        ui->lineEditSerialHDD->setText("");
        return void();
    }

    ui->lineEditSerialHDD->setText(device.getSerialDevice("SCSI"));

    if(ui->lineEditSerialHDD->text() == ""){

        QMessageBox msgBox;
        msgBox.setWindowIcon(QIcon(QPixmap(":/png/5.png")));
        msgBox.setWindowTitle("Ошибка");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText("Не удалось обнаружить устройство");
        msgBox.exec();

    }else{
        ui->pushButtonAddIO_2->setText("Удалить");
    }
}
