#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("CryptoFile");
    setWindowIcon(QIcon(":/png/ICON_CRYPTO.png"));

}

MainWindow::~MainWindow()
{
    ui->lineKey->setText("XXXXXXXXXXXXXXXXXX");

    delete ui;
}


void MainWindow::on_pushButton_2_clicked()

{
    log.startMovie();

    if(Cryptograph::checkKey(ui->lineKey->text()) != 0){
        ui->status->setText("Ключ слишком простой");
        return void();
    }


    bool encryptAction;
    if(ui->comboBox->currentIndex() == 0){
        encryptAction = true;
    } else{
        encryptAction = false;
    }

    FileManager fileManager(settings.getParams());

    fileManager.cryptoFolder(ui->lineDir->text(),ui->lineKey->text(),encryptAction);


    log.stopMovie();
    if(encryptAction){
        ui->status->setText("Зашифровано");
    } else {
        ui->status->setText("Дешифровано");
    }

}

void MainWindow::on_pushButton_clicked()
{


    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

    ui->lineDir->setText(dir);
}

void MainWindow::on_pushButtonKeyGen_clicked()
{
    Parametrs param = settings.getParams();

    int keyLenght;
    switch(param.algID){
    case 0:
        keyLenght = 16;
        break;

    case 1:
        keyLenght = 24;
        break;

    case 2:
        keyLenght = 32;
        break;
    }

    ui->lineKey->setText(Cryptograph::keyGen(keyLenght));
}

void MainWindow::on_pushButtonSettings_clicked()
{

    settings.show();
}

void MainWindow::on_lineKey_textEdited(const QString &arg1)
{
    if(ui->comboBox->currentIndex() == 1){ // при дешифровании не проверяем ключ
        return void();
    }

    int status = Cryptograph::checkKey(arg1);

    switch (status)
    {
    case 0:
        ui->keyCheck->setText("");
        break;
    case 1:
        ui->keyCheck->setText("Слишком короткий ключ");
        break;
    case 2:
        ui->keyCheck->setText("Требуется верхний регистр");
        break;
    case 3:
        ui->keyCheck->setText("Требуется цифра");
        break;
    case 4:
        ui->keyCheck->setText("Требуется нижний регистр");
        break;
    case 5:
        ui->keyCheck->setText("Введите ключ");
        break;
    }
}




















