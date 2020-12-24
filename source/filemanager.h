#ifndef BACKUPMANAGER_H
#define BACKUPMANAGER_H
#include <QString>
#include <QFile>
#include <QDir>
#include <QMap>

#include <queue>
#include <stack>
#include <future>

#include "settings.h"
#include "cryptograph.h"


class FileManager
{
public:
    FileManager();
    FileManager(const Parametrs &parametrs);

    void cryptoFolder(QString pathFolder, QString key, bool action);

    QMap<QString, size_t> copyFilesDir(QString path , bool needCopyFiles = false);
     bool checkFiles(size_t sizeFile ,QString path);
     void replaceFile(QFile &file , QByteArray &byteArray);
     void getData(QFile &file,QByteArray &byteArray);

     int backupFolder();

private:
    int encryptFile(QString path,Cryptograph *cryptograph,size_t size);
    int decryptFile(QString path,Cryptograph *cryptograph,size_t size);

    void copyFilesPath(QString path, QMap<QString, size_t> &filesDir , QString dst = "");

    Parametrs m_parametrs;
    bool m_rezervFiles;
    quint64 totalSize;
};

#endif // BACKUPMANAGER_H
