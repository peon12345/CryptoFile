#ifndef AESCBC_H
#define AESCBC_H
#include "aes.h"

class AesCbc : public AES
{
public:

    AesCbc() = delete;
    AesCbc(QString &key,QString &IV,int keyLenght);
    AesCbc(QString &key,int keyLenght);

    void encrypt(uint8_t input[16],  uint8_t output[]) override;
    void decrypt( uint8_t input[16],  uint8_t *output) override;

    void convertAndSetIV(QString &IV);

    void  getPointersToLock(QMap<uint8_t*,size_t>& ptrsForLock) override;
    int getSizeMetaData() override;

    QByteArray getMetaData() override;
    void setMetaData(QByteArray &input) override;

   private:

    void StateXorPrevState(uint8_t state[4][4]);
    void generateInitialVec();

    uint8_t m_prevState[4][4];
    uint8_t buffer[4][4];
    bool isIVgenerated;

};

#endif // AESCBC_H
