#ifndef ENCRYPT_H
#define ENCRYPT_H
#include <QString>
#include <cmath>
#include <memory>
#include <QFile>
#include <QByteArray>
#include <QDir>

#include "aescbc.h"
#include "memorymanager.h"

class Cryptograph
{
public:
    explicit Cryptograph( QString &key ,QString &IV,int keyLenght);
    explicit Cryptograph( QString &key ,QString encryptionType = "AES");
    Cryptograph() = delete;

    void encryptFile(QByteArray& input,QByteArray& output );
    void decryptFile(QByteArray& input,QByteArray& output );

    void writeNeedDelete(QByteArray &input ,const size_t size);

    void getMetaData(QByteArray &input);
    void setMetaData(QByteArray &input);

    int getSizeMetaData();
    int getBlockSize();

    static QString keyGen(int keyLenght);
    static int checkKey( const QString &key);

    Cryptograph(const Cryptograph&) = delete;
    Cryptograph& operator=(const Cryptograph&) = delete;
    ~Cryptograph();


private:
     Ialgorithm *m_alg;
     MemoryManager m_memoryManager;
};

#endif // ENCRYPT_H
