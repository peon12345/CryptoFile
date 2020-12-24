#include "filemanager.h"

FileManager::FileManager()
{

}

FileManager::FileManager(const Parametrs &parametrs) : m_parametrs(parametrs)
{
    FileManager();
}

int FileManager::encryptFile(QString path,Cryptograph *cryptograph,size_t size){

    QFile file(path);

    if(!file.open(QIODevice::ReadWrite)){
        //создать лог и сунуть  ошибку

        return 1;
    }


    QByteArray output;

    cryptograph->getMetaData(output);

    while(!file.atEnd()){

        QByteArray data;

        getData(file,data);

        cryptograph->encryptFile(data,output);

        if(file.atEnd()) {
            cryptograph->writeNeedDelete(output,size);
        }
    }

    replaceFile(file,output);

    return 0;
}


int FileManager::decryptFile(QString path, Cryptograph *cryptograph,size_t size){

    QFile file(path);

    if(!file.open(QIODevice::ReadWrite)){
        //создать лог и показать  ошибку

        return 1;
    }


    //удалить из инпут IV , подумать об остальных алг.

    QByteArray output;

    cryptograph->getMetaData(output);

    if(output.size() != 0){

    int blockSize = cryptograph->getBlockSize();
    int sizeBlockMetaData = cryptograph->getSizeMetaData();

    if( cryptograph->getSizeMetaData() > 0){
        blockSize = blockSize * sizeBlockMetaData ;
    }

    QByteArray metaData;
    metaData = file.read(blockSize);
    cryptograph->setMetaData(metaData);

    }

    while(!file.atEnd()){

        QByteArray data;

        getData(file,data);

        cryptograph->decryptFile(data,output);
    }

    replaceFile(file,output);

    return 0;
}


void FileManager::cryptoFolder(QString pathFolder, QString key, bool action){

    QString IV;
    IV += m_parametrs.serialUSB;
    IV += m_parametrs.serialHDD;

    std::stack<std::unique_ptr<Cryptograph>> stackCrypt;

    QMap<QString, size_t> filesDir = copyFilesDir(pathFolder ,m_parametrs.copy );

    std::queue<std::future<int>> queueTask;

    QMap<QString, size_t>::const_iterator itPath = filesDir.constBegin();
    while (itPath != filesDir.constEnd()) {

        QString path = itPath.key();
        size_t  size = itPath.value();

        //выбираем алгоритм шифрования
        switch(m_parametrs.algID){
        case 0:
            stackCrypt.push(std::make_unique<Cryptograph>(key,IV,128));
            break;

        case 1:
            stackCrypt.push(std::make_unique<Cryptograph>(key,IV,192));
            break;

        case 2:
            stackCrypt.push(std::make_unique<Cryptograph>(key,IV,256));
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
        queueTask.pop();
    }

}

void FileManager::getData(QFile &file,QByteArray &byteArray){

    byteArray = file.read(16000000); //16 mb

}

bool FileManager::checkFiles(size_t size,QString path){

    std::reverse(path.begin(), path.end());

    QString fileName;
    for(int i = 0; i < path.length();i++){
        if(path[i] == QDir::separator()) {
            break;
        }
        fileName += path[i];
    }

    QString pathBackup = QDir::currentPath() + QDir::separator() + "/backup";
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


QMap<QString, size_t> FileManager::copyFilesDir(QString pathFolder , bool needCopyFiles){

    if(needCopyFiles){
        m_rezervFiles = true;
    } else{
        m_rezervFiles = false;
    }

    totalSize = 0;

    QMap<QString, size_t> filesDir;
    copyFilesPath(pathFolder,filesDir);

    return filesDir;

}

void FileManager::copyFilesPath(QString path ,QMap<QString, size_t> &filesDir , QString dst){

    static QString currentDir = QDir::currentPath();
    currentDir += "/backup";

    QDir dir(path);
    if (! dir.exists())
        return void();


    foreach (QString d, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QString dst_path = currentDir + QDir::separator() + d;
        dir.mkpath(dst_path);
        copyFilesPath(path+ QDir::separator() + d,filesDir , dst_path);
    }

    QFileInfoList folderitems( dir.entryInfoList() );

    foreach ( QFileInfo i, folderitems ) {
        QString iname( i.fileName() );
        if ( iname == "." || iname == ".." || iname.isEmpty() ){
            continue;
        }

        if (m_rezervFiles){
            QFile::copy(path + QDir::separator() + i.fileName(), dst + QDir::separator() + i.fileName());
        }
        if(i.isFile()){
            filesDir.insert(path + QDir::separator() + i.fileName(),i.size()  );
        }

        totalSize += i.size();
    }

}
