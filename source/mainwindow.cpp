#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("CryptoFile");
    setWindowIcon(QIcon(":/png/res/ICON_CRYPTO.png"));

    connect(&fileManager, &FileManager::complete, &m_log, &Log::showResult);
    connect(&fileManager, &FileManager::sendMessage, &m_log, &Log::addMessage);
    connect(&fileManager, &FileManager::compliteStep, &m_log, &Log::addProgress);
    connect(&m_log,&Log::needBackUp,&fileManager,&FileManager::backUpFolder);
    connect(&m_log,&Log::needClearBackUpFolder,&fileManager,&FileManager::clearBackUpFolder);

    if(!QDir(QDir::currentPath() + QDir::separator() + FileManager::BACK_UP_FOLDER_NAME).exists()){
        QDir().mkdir(QDir::currentPath() + QDir::separator() + FileManager::BACK_UP_FOLDER_NAME);
    }

}

MainWindow::~MainWindow()
{
    ui->lineKey->setText("XXXXXXXXXXXXXXXXXX");
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{
    if(Cryptograph::checkKey(ui->lineKey->text(),settings.getAlgID()) != 0 ){
        ui->keyCheck->setText("Ключ слишком простой");
        return void();
    }

    settings.setPath(ui->lineDir->text());

    bool encryptAction;
    QString actionStr;
    if(ui->comboBox->currentIndex() == 0){
        encryptAction = true;
        actionStr = "Шифрование папки "+ui->lineDir->text()+"";
    } else{
        encryptAction = false;
        actionStr = "Дешифрование папки "+ui->lineDir->text()+"";
    }

    m_log.showLog(actionStr);

    fileManager.updateParams(settings.getParams());

    std::thread t(&FileManager::cryptoFolder,std::ref(fileManager),ui->lineDir->text(),ui->lineKey->text(),encryptAction);
    t.detach();

    //проверка на корректность шифра
    //проверка на миссклик шифrе\дешифр

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
    ui->lineKey->setText(Cryptograph::keyGen(settings.getAlgID()));
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

    int status = Cryptograph::checkKey(arg1,settings.getAlgID());

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




