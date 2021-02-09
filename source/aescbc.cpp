#include "aescbc.h"

AesCbc::AesCbc(QString key,QString IV,int keyLenght) : AES(keyLenght),isIVgenerated(false),isIVpasted(true)
{
    convertAndSetIV(IV);

    setKey(key);
}

AesCbc::AesCbc(QString key,int keyLenght,bool generateIV) : AES(keyLenght),isIVgenerated(generateIV),isIVpasted(false)
{
    if(isIVgenerated){
        generateInitialVec();
    }
    setKey(key);
}

QByteArray AesCbc::getMetaData(){

    QByteArray IV;

    if(isIVgenerated || isIVpasted){
        for(int i = 0;i<4;i++){
            for(int j = 0;j<4;j++){
                IV.append(m_prevState[i][j]);
            }
        }
    }

    return IV;
}


void AesCbc::setMetaData(QByteArray &input){

    for(int i = 0;i<4;i++){
        for(int j = 0;j<4;j++){
            m_prevState[i][j] = input.at((4 * i)  + j);
        }
    }

}

int AesCbc::getSizeMetaData(){

    return 1;
}


void AesCbc::encrypt(uint8_t input[16], uint8_t output[] )  {

    uint8_t state [4][4]=
    {   {input[0], input[4], input[8], input[12]},
        {input[1], input[5], input[9], input[13]},
        {input[2], input[6], input[10], input[14]},
        {input[3], input[7], input[11], input[15]}  };

    StateXorPrevState(state);

    for(int i = 0;i < 4;i++){
        for(int j =0; j<4;j++){

            m_roundKey[i][j] = m_wKey[(j * 4) + i ];
        }

    }

    AddRoundKey(state,m_roundKey);

    for(int round = 1 ;round <= m_round ; round++){

        for (uint8_t i = 0; i < 4; i++)
        {
            for (uint8_t j = 0; j < 4; j++)
            {

                m_roundKey[i][j] = m_wKey[(round * blockSize) + (j * 4) + i ];
            }
        }

        SubBytes(state);
        ShiftRows(state);

        if(round != m_round){
            MixColumn(state);
        }

        AddRoundKey(state,m_roundKey);
    }

    // Сохраняем этот стейт
    for (uint8_t i = 0; i < 4; i++)
    {
        for (uint8_t j = 0; j < 4; j++)
        {
            m_prevState[i][j] = state[i][j];

        }
    }

    //выводим результат
    for (uint8_t i = 0; i < 4; i++)
    {
        for (uint8_t j = 0; j < 4; j++)
        {
            output[(j * 4) + i] = state[i][j];

        }
    }

}

void AesCbc::decrypt(uint8_t input[16], uint8_t *output){

    uint8_t state [4][4]=
    {   {input[0], input[4], input[8], input[12]},
        {input[1], input[5], input[9], input[13]},
        {input[2], input[6], input[10], input[14]},
        {input[3], input[7], input[11], input[15]}  };

    for(int i = 0;i < 4;i++){
        for(int j =0; j<4;j++){

            buffer[i][j] = state[i][j];
        }
    }

    //заполняем раундовый ключ с конца расширенного ключа
    for(int i = 0;i < 4;i++){
        for(int j =0; j<4;j++){

            m_roundKey[i][j] = m_wKey[ (m_wKeyLenght - 16)+ (j * 4) + i ];
        }

    }

    AddRoundKey(state,m_roundKey); //INITIAL ROUND
    invShiftRows(state);
    invSubBytes(state);

    for(int round = m_round-1  ; round > 0; round--){

        for (uint8_t i = 0; i < 4; i++)
        {
            for (uint8_t j = 0; j < 4; j++)
            {
                m_roundKey[i][j] = m_wKey[(round * blockSize) + (j * 4) + i ];
            }
        }

        AddRoundKey(state,m_roundKey);
        invMixColumn(state);
        invShiftRows(state);
        invSubBytes(state);

    }

    for (uint8_t i = 0; i < 4; i++)
    {
        for (uint8_t j = 0; j < 4; j++)
        {
            m_roundKey[i][j] = m_wKey[  (j * 4) + i ];
        }
    }

    AddRoundKey(state,m_roundKey);

    StateXorPrevState(state);

    for (uint8_t i = 0; i < 4; i++)
    {
        for (uint8_t j = 0; j < 4; j++)
        {
            m_prevState[i][j] = buffer[i][j];

        }

        for (uint8_t i = 0; i < 4; i++)
        {
            for (uint8_t j = 0; j < 4; j++)
            {
                output[(j * 4) + i] = state[i][j];
            }
        }

    }
}

void  AesCbc::getPointersToClear(QMap<uint8_t*,size_t>& ptrsForLock){

    ptrsForLock.insert(m_key,16);
    ptrsForLock.insert(m_wKey,m_wKeyLenght);
    ptrsForLock.insert(*m_prevState,blockSize);
    ptrsForLock.insert(*m_roundKey,blockSize);
}


void AesCbc::generateInitialVec()  {
    std::mt19937 m_gen;
    m_gen.seed(time(0));
    std::default_random_engine generator;
    std::uniform_int_distribution<int> lenghtRand(1, 6);

    QString valueStr;
    for(int j = 0; j < 4; j++){

        for(int k = 0; k<4; k++){

            QString valueStr;
            int lenghtNum = lenghtRand(m_gen);

            for(int i = 0; i <= lenghtNum; i++){

                std::uniform_int_distribution<int> valueRand(0, 1);
                valueStr += QString::number(valueRand(m_gen));
            }
            m_prevState[j][k] = valueStr.toUInt();
        }
    }
}

void AesCbc::convertAndSetIV(QString IV){

    if(IV == ""){
        return void();
    }

    QByteArray IVarray(IV.toUtf8());

    for(int k = 0; k<4;k++){

        for(int s =0; s<4; s++){

            if((k * 4) + s < IV.size()){
                m_prevState[s][k] =  IVarray[(k * 4) + s];
            } else {

                m_prevState[s][k] = IVarray[ ((k * 4) + s) - IV.size()];
            }

            if(k==3 && s==3 && IV.size() > 16){

                for(int i = 0; i<4;i++){

                    for (int j = 0;j <4 ;j++ ) {

                        if(((k * 4) + s) + ((i * 4) + j) < IV.size()){
                            m_prevState[i][j] -= IVarray[ ((k * 4) + s) + ((i * 4) + j)];
                        }
                    }
                }
            }
        }
    }
}


void AesCbc::StateXorPrevState(uint8_t state[4][4]){

    for (uint8_t i = 0; i < 4; i++)
    {
        for (uint8_t j = 0; j < 4; j++)
        {
            state[i][j] ^= m_prevState[i][j];
        }
    }

}
