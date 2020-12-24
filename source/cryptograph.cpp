#include "cryptograph.h"


Cryptograph::Cryptograph(QString &key , QString &IV , int keyLenght)
{

    if(IV.size() == 0){
        m_alg = new AesCbc(key,keyLenght);
    } else{
        m_alg = new AesCbc(key,IV,keyLenght);
    }

    m_alg->getPointersToLock(m_memoryManager.getLockPtrs());
    m_memoryManager.lockAll();
}




Cryptograph::~Cryptograph()
{
    delete m_alg;
}


int Cryptograph::checkKey( const QString &key){
    return AES::checkKey(key);
}


void Cryptograph::writeNeedDelete(QByteArray& input , size_t size){

    uint8_t countDeleteNull = 16 - ( size % blockSize );

    input.append(countDeleteNull);
}

QString Cryptograph::keyGen(int keyLenght){


    std::default_random_engine generator;

    std::mt19937 m_gen;
    m_gen.seed(time(0));

    std::uniform_int_distribution<uint8_t> randValue(0,255);
    QString key;

    for(int j = 0; j < keyLenght; j++){

        uint8_t num = randValue(m_gen);

        key += num;
    }

    int statusKey = 1;

    while(statusKey != 0){

        statusKey = checkKey(key);

        switch (statusKey) {

        case 0: {

            return key;
        }
            break;


        case 2: {
            std::uniform_int_distribution<uint8_t> randPos(0,keyLenght-1);
            uint8_t pos = randPos(m_gen);

            key[pos] = key[pos].toUpper();

            int counterLenght = 0;

            while(key[pos].isLower() == key[pos].toUpper()){

                if(counterLenght == keyLenght){
                    return ""; // значит ни один символ не перевелся
                    break;
                }


                if(pos != keyLenght){
                    pos++;
                    counterLenght++;
                } else {
                    pos = 0;
                    counterLenght++;
                }

                key[pos] = key[pos].toUpper();
            }

        }
            break;

        case 3: {
            std::uniform_int_distribution<uint8_t> randPos(0,keyLenght-1);
            std::uniform_int_distribution<uint8_t> number(48,57);
            uint8_t pos = randPos(m_gen);

            uint8_t value = number(m_gen);

            key[pos] = value;

        }
            break;

        case 4:{

            std::uniform_int_distribution<uint8_t> randPos(0,keyLenght-1);
            uint8_t pos = randPos(m_gen);

            key[pos] = key[pos].toLower();

            int counterLenght = 0;

            while(key[pos].isUpper() != key[pos].toLower()){

                if(counterLenght == keyLenght){
                    return ""; // значит ни один символ не перевелся
                    break;
                }

                if(pos != 15){
                    pos++;
                    counterLenght++;
                } else {
                    pos = 0;
                    counterLenght++;
                }

                key[pos] = key[pos].toLower();
            }
        }
            break;
        }
    }

    return key;
}

void Cryptograph::encryptFile(QByteArray& input , QByteArray& output  ){

    //делим на блоки
    int blockSize = m_alg->getBlockSize();

    int counterState = std::ceil((float)input.size() / blockSize);
    int inputIterator = 0;

    uint8_t encryptedState[blockSize];

    for (int i = 1; i <= counterState; i++) {

        uint8_t state[blockSize];

        for (uint8_t k = 0; k < blockSize; k++)
        {
            if(inputIterator < input.size()){

                state[k] = input.at(inputIterator);
                inputIterator++;
            } else{

                state[k] = 0;
            }
        }

        std::fill(encryptedState, encryptedState + blockSize, 0);
        m_alg->encrypt(state,encryptedState );

        for(int j = 0; j < blockSize; j++ ){
            //записываем шифованный файл
            output.append(encryptedState[j]);
        }
    }
}

int Cryptograph::getSizeMetaData(){

    return m_alg->getSizeMetaData();
}

void Cryptograph::setMetaData(QByteArray &input){

    m_alg->setMetaData(input);
}


void Cryptograph::getMetaData(QByteArray &input){

    input += m_alg->getMetaData();
}

int Cryptograph::getBlockSize(){

    return m_alg->getBlockSize();
}


void Cryptograph::decryptFile(QByteArray &input, QByteArray &output){
    //рзделяем на блоки
    int counterState = std::ceil((float)(input.size()-1) / blockSize);
    int inputIterator = 0;

    int blockSize = m_alg->getBlockSize();

    uint8_t encryptedState[blockSize];

    uint8_t deleteLastNull = input.at(input.size()-1);


    for (int i = 1; i <= counterState; i++) {

        uint8_t state[blockSize];

        for (uint8_t k = 0; k < blockSize; k++)
        {
            if(inputIterator < input.size() - 1 ){

                state[k] = input.at(inputIterator);

                inputIterator++;
            } else{
                //если блок не делится на 16 без остатка, то у нас проблемы
                break;
            }
        }

        std::fill(encryptedState, encryptedState + blockSize, 0);

        m_alg->decrypt(state,encryptedState );

        if(i!= counterState){
            for(int j = 0; j < blockSize; j++ ){

                output.append(encryptedState[j]);

            }
        } else{

            for(int j = 0; j < (blockSize - deleteLastNull); j++ ){

                output.append(encryptedState[j]);

            }
        }
    }
}
