#ifndef IALGORITHM_H
#define IALGORITHM_H
#include <QByteArray>

class Ialgorithm {

public:

   virtual void encrypt(uint8_t input[],uint8_t output[]) =0;
   virtual void decrypt(uint8_t input[],uint8_t output[]) =0;

   virtual void setKey(QString &key) = 0;

   virtual int getBlockSize() = 0;
   virtual int getSizeMetaData() = 0;

   virtual QByteArray getMetaData() = 0;
   virtual void setMetaData(QByteArray &input) = 0;

   virtual void getPointersToLock(QMap<uint8_t*,size_t>& ptrsForLock) = 0;

   virtual ~Ialgorithm() {};

};

#endif // IALGORITHM_H
