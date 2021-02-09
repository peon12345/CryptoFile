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


class FileManager : public QObject
{
    Q_OBJECT
public:
    FileManager();
    FileManager(const Parametrs &parametrs,QObject *parent = nullptr);

    void updateParams(Parametrs &parametrs);

    void cryptoFolder(QString pathFolder, QString key, bool action);

    QMap<QString, size_t> copyFilesDir(QString path);
    void deleteFolder(QString path);

    bool checkFiles(size_t sizeFile ,QString path);
    void replaceFile(QFile &file , QByteArray &byteArray);

    static inline const QString BACK_UP_FOLDER_NAME = "BackupFiles";

private:

    int encryptFile(QString path,Cryptograph *cryptograph,size_t size);
    int decryptFile(QString path,Cryptograph *cryptograph,size_t size = 0);

    void copyFilesPath(QString path, QMap<QString, size_t> &filesDir , QString dst = "" , bool backUp = false);

    Parametrs m_parametrs;
    quint64 totalSize;

signals:
    void sendMessage(QString message,int type = 0);
    void compliteStep(int percent);
    void complete();

public slots:
    void clearBackUpFolder();
    void backUpFolder();

};

#endif // BACKUPMANAGER_H
