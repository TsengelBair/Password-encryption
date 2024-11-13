#ifndef PASSWORDENCRYPTOR_H
#define PASSWORDENCRYPTOR_H

#include <QVector>
#include <QString>

class PasswordEncryptor
{
public:
    /*
        param - пароль, который нужно зашифровать
        сохраняем переданный пароль как m_password в конструкторе
        расшифровать зашифрованный пароль, можно только зная исходный пароль
    */
    PasswordEncryptor(QString password);
    ~PasswordEncryptor() = default;
    /*
        В методе code можем обращаться только к паролю, установленному в m_password
    */
    void code();
    void decode();
    /*
        Для последующего декода необходимо знать изначальный seed
        поэтому сохраянем в m_originalSeed
    */
    void setOriginalSeed(int seed);

private:
    // метод, преобращующий пароль (строку) в массив int
    QVector<int>makeAsciiArray();
    // метод для вычисления начального seed
    // param - вектор рассчитанный в предыдущем методе
    int calcSeed(const QVector<int>&asciiArray);
    // псевдорандом
    int getRand(int& seed, int size);

private:
    QString m_password;
    int m_originalSeed;
    QVector<int>m_cryptedArray;
};

#endif // PASSWORDENCRYPTOR_H
