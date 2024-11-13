#include <algorithm>
#include <QDebug>

#include "passwordencryptor.h"

/* Константы для генерации псевдослучайных чисел */
const int a = 401;
const int c = 179;

PasswordEncryptor::PasswordEncryptor(QString password) : m_password(password)
{
}

QVector<int> PasswordEncryptor::makeAsciiArray()
{
    QVector<int>asciiArray(m_password.length());
    for (int i = 0; i < m_password.length(); ++i){
        asciiArray[i] = m_password[i].unicode();
    }
    return asciiArray;
}

int PasswordEncryptor::calcSeed(const QVector<int> &asciiArray)
{
    int seed = 0;
    for (int val : asciiArray) {
        seed += val;
    }
    return seed;
}

void PasswordEncryptor::setOriginalSeed(int seed)
{
    m_originalSeed = seed;
}

int PasswordEncryptor::getRand(int &seed, int size)
{
    seed = (seed * a + c) % size;
    return seed;
}

void PasswordEncryptor::code()
{
    m_cryptedArray = makeAsciiArray();

    int seed = calcSeed(m_cryptedArray);
    setOriginalSeed(seed);

    int indexToSwap;
    for (int i = 0; i < m_cryptedArray.size(); ++i){
        indexToSwap = getRand(seed, m_cryptedArray.size());
        std::swap(m_cryptedArray[i], m_cryptedArray[indexToSwap]);
    }
}

void PasswordEncryptor::decode()
{
    // Восстанавливаем начальный seed
    int seed = m_originalSeed;

    // Воссоздаем индексы для обратного восстановления порядка
    QVector<int>swapIndices(m_cryptedArray.size());
    for (int i = 0; i < m_cryptedArray.size(); ++i){
        swapIndices[i] = getRand(seed, m_cryptedArray.size());
    }

    // Используем воссозданные индексы перестановок в обратном порядке
    for (int i = m_cryptedArray.size() - 1; i >= 0; --i) {
        std::swap(m_cryptedArray[i], m_cryptedArray[swapIndices[i]]);
    }

    // Для теста
    QString decodedPassword;
    for (int val : m_cryptedArray){
        decodedPassword += QChar(val);
    }
    qDebug() << decodedPassword;
}
