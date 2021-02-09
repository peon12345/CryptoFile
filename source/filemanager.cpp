#include "filemanager.h"

FileManager::FileManager()
{

}

FileManager::FileManager(const Parametrs &parametrs,QObject *parent) :QObject(parent) ,m_parametrs(parametrs)
{
}

void FileManager::updateParams(Parametrs &parametrs){
    m_parametrs = parametrs;
}

int FileManager::encryptFile(QString path,Cryptograph *cryptograph,size_t size){

    QFile file(path);

    if(!file.open(QIODevice::ReadWrite)){
        emit sendMessage("Не удалось открыть файл "+path+".",3);
        return 1;
    }

    QByteArray output;

    if(cryptograph->isGeneratedData()){
        cryptograph->getMetaData(output);
    }

    while(!file.atEnd()){

        QByteArray data;
        data = file.read(16000000); //16 mb

        cryptograph->encryptFile(data,output);

        int percent = std::ceil(static_cast<double>(data.size()) / static_cast<double>(totalSize) * 100) ;
        emit compliteStep(percent);

        if(file.atEnd()) {
            cryptograph->writeNeedDelete(output,size);
        }
    }

    replaceFile(file,output);

    emit sendMessage("Файл "+path+" Зашифрован.",1);

    return 0;
}


int FileManager::decryptFile(QString path,Cryptograph *cryptograph,size_t size){

    QFile file(path);

    if(!file.open(QIODevice::ReadWrite)){
        emit sendMessage("Не удалось открыть файл "+path+".",3);
        return 1;
    }

    QByteArray metaData;

    cryptograph->getMetaData(metaData);

    if(metaData.size() == 0){

        int blockSize = cryptograph->getBlockSize();
        int sizeBlockMetaData = cryptograph->getSizeMetaData();

        if( cryptograph->getSizeMetaData() > 0){
            blockSize = blockSize * sizeBlockMetaData;

            metaData = file.read(blockSize);
            cryptograph->setMetaData(metaData);
        }
    }

    QByteArray output;

    while(!file.atEnd()){

        QByteArray data;
        data = file.read(16000000); //16 mb

        cryptograph->decryptFile(data,output);

        int percent = std::ceil(static_cast<double>(data.size()) / static_cast<double>(totalSize) * 100) ;
        emit compliteStep(percent);
    }

    replaceFile(file,output);

    emit sendMessage("Файл "+path+" Дешифрован.",1);
    return 0;
}


void FileManager::cryptoFolder(QString pathFolder, QString key, bool action){
    QString IV;
    IV += m_parametrs.serialUSB;
    IV += m_parametrs.serialHDD;

    std::stack<std::unique_ptr<Cryptograph>> stackCrypt;

    QMap<QString, size_t> filesDir = copyFilesDir(pathFolder);

    if(filesDir.empty()){
        emit sendMessage("Файлы в папке "+pathFolder+" не найдены!");
        return void();
    }

    std::queue<std::future<int>> queueTask;

    //нужен ли вектор инициализации
    bool generateIV = false;

    if(IV == "" && action == true){
        generateIV = true;
    }else{
        generateIV = false;
    }

    QMap<QString, size_t>::const_iterator itPath = filesDir.constBegin();
    while (itPath != filesDir.constEnd()) {

        QString path = itPath.key();
        size_t  size = itPath.value();

        //выбираем алгоритм шифрования
        switch(m_parametrs.algID){
        case 0:
            stackCrypt.push(std::make_unique<Cryptograph>(key,IV,128,generateIV));
            break;
        case 1:
            stackCrypt.push(std::make_unique<Cryptograph>(key,IV,192,generateIV));
            break;
        case 2:
            stackCrypt.push(std::make_unique<Cryptograph>(key,IV,256,generateIV));
            break;
        }

        if(action == true){
            queueTask.push(std::async(std::launch::async,&FileManager::encryptFile,this,path,stackCrypt.top().get(),size));
        } else{
            queueTask.push(std::async(std::launch::async,&FileManager::decryptFile,this,path,stackCrypt.top().get(),size));
        }

        itPath++;
    }

    // нужно как то проверять шифровку
    while(!queueTask.empty()){
        auto &task = queueTask.front();
        bool status = task.get();
        //добавить проверку внутрь на шифр\дешифр
        queueTask.pop();
    }

    emit complete();
}

//доработать
bool FileManager::checkFiles(size_t size,QString path){

    std::reverse(path.begin(), path.end());

    QString fileName;
    for(int i = 0; i < path.length();i++){
        if(path[i] == QDir::separator()) {
            break;
        }
        fileName += path[i];
    }

    QString pathBackup = QDir::currentPath() + QDir::separator() + BACK_UP_FOLDER_NAME;
    pathBackup += QDir::separator() + fileName;

    size_t sizeBackupFile = QFileInfo(pathBackup).size(); // получили размер сохраненного файла

    if (sizeBackupFile == size){
        return true;
    }else{
        return false;
    }

}

void FileManager::replaceFile(QFile &file, QByteArray &byteArray){

    file.resize(0);
    file.write(byteArray);
    file.close();
}

void FileManager::clearBackUpFolder(){

    QString path = QDir::currentPath() + QDir::separator() + BACK_UP_FOLDER_NAME;
    deleteFolder(path);
    QDir().mkdir(path);
}

void FileManager::deleteFolder(QString path){
    QDir dir(path);
    dir.removeRecursively();
}


QMap<QString, size_t> FileManager::copyFilesDir(QString pathFolder){

    if(m_parametrs.copy){
        emit sendMessage("Копирование файлов включено.");
    } else{
        emit sendMessage("Копирование файлов отключено.",2);
    }

    totalSize = 0;

    QMap<QString, size_t> filesDir;
    copyFilesPath(pathFolder,filesDir);

    return filesDir;

}

void FileManager::backUpFolder(){

    deleteFolder(m_parametrs.path);

    QMap<QString, size_t> filesDir;
    QDir backUpDir = QDir::currentPath() + QDir::separator() + BACK_UP_FOLDER_NAME;
    copyFilesPath(backUpDir.absolutePath(),filesDir,"",true);
}

void FileManager::copyFilesPath(QString path ,QMap<QString, size_t> &filesDir , QString dst, bool backUp){

    QDir dir(path);
    if (! dir.exists())
        return void();

    static QString backUpDir = QDir::currentPath() + QDir::separator() + BACK_UP_FOLDER_NAME;
    static QString replacePath = m_parametrs.path;

    foreach (QString d, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QString dst_path = QDir::currentPath() + QDir::separator() + d;
        if (m_parametrs.copy){
            backUpDir.clear();
            backUpDir = QDir::currentPath() + QDir::separator() + BACK_UP_FOLDER_NAME + QDir::separator() + d;
            QDir().mkdir(backUpDir);


            if(backUp){
                replacePath.clear();
                replacePath = m_parametrs.path + QDir::separator() + d;
                QDir().mkdir(replacePath);
            }
        }
        //dir.mkpath(dst_path);
        copyFilesPath(path+ QDir::separator() + d,filesDir,dst_path,backUp);
    }

    QFileInfoList folderitems(dir.entryInfoList());

    foreach ( QFileInfo i, folderitems ) {
        QString iname( i.fileName() );
        if ( iname == "." || iname == ".." || iname.isEmpty() ){
            continue;
        }

        if (m_parametrs.copy){
            emit sendMessage("Копирование файла "+(path + QDir::separator() + i.fileName()+"..."));
            if(backUp){
                QFile::copy(backUpDir + QDir::separator() + i.fileName(),replacePath + QDir::separator() + i.fileName());
            }
            else
                QFile::copy(path + QDir::separator() + i.fileName(), backUpDir + QDir::separator() + i.fileName());
        }
        if(i.isFile()){

            filesDir.insert(path + QDir::separator() + i.fileName(),i.size());
            totalSize += i.size();

        }
    }

    backUpDir = QDir::currentPath() + QDir::separator() + BACK_UP_FOLDER_NAME;
    replacePath = m_parametrs.path;

}
