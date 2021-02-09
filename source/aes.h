#ifndef AES_H
#define AES_H

#include <iostream>
#include <QString>
#include <random>
#include <ctime>

#include "Ialgorithm.h"
#include "memorymanager.h"



class AES : public Ialgorithm
{
public:

    AES() = delete;
    AES(int keyLenght);

    virtual ~AES();

    void setKey(QString key) override;
    int getBlockSize() override;

    static int checkKey( const QString key);

    static constexpr int blockSize = 16;

private:

    //количество раундов
    static constexpr int ROUND_128 = 10;
    static constexpr int ROUND_192 = 12;
    static constexpr int ROUND_256 = 14;

//    //длина ключа
//    static constexpr int KEY_LENGHT_128 = 16;
//    static constexpr int KEY_LENGHT_192 = 24;
//    static constexpr int KEY_LENGHT_256 = 32;
    static constexpr int KEY_LENGHT = 16;


    //длина расширенного ключа
    static constexpr int EXPANDED_KEY_LENGTH_128 = 176;
    static constexpr int EXPANDED_KEY_LENGTH_192 = 208;
    static constexpr int EXPANDED_KEY_LENGTH_256 = 240;

protected:

    void convertToKeyUInt8(QString input);

    int m_round;

    uint8_t m_key[KEY_LENGHT];
   // int m_keyLenght;

    uint8_t *m_wKey;
    int m_wKeyLenght;

    uint8_t m_roundKey[4][4];

    void KeyExpansion();
    void AddRoundKey(uint8_t state[4][4],uint8_t roundKey[4][4]);

    //методы шифрования
    void SubBytes (uint8_t state[4][4]);
    void SubBytes (uint8_t keyColumn[4]);
    void ShiftRows(uint8_t state[4][4]);
    void MixColumn(uint8_t state[4][4]);

    //метода дешифрования
    void invSubBytes (uint8_t state[4][4]);
    void invSubBytes (uint8_t keyColumn[4]);
    void invShiftRows(uint8_t state[4][4]);
    void invMixColumn(uint8_t state[4][4]);
};

#endif // AES_H
