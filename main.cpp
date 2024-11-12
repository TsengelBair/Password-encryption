#include <iostream>
#include <vector>
#include <string>

/* Константы для генерации псевдослучайных чисел */
const int a = 401;
const int c = 179;

/* Класс для шифрования паролей */
class PasswordEncryptor {
public:
    PasswordEncryptor(std::string password = "test") : m_password(password), m_originalSeed(0) {};
    ~PasswordEncryptor() = default;
    /*
        Расшифровать пароль можно только зная исходный seed
        для последующего декода сохраняем seed в сеттере
    */
    void setOriginalSeed(int seed) {
        m_originalSeed = seed;
    }
    /*
        Шифруем -> получаем зашифрованный массив и сохраняем в m_cryptedArray
    */
    void code(const std::string& originalPassword) {
        // Каждый символ пароля преобразуем в значение из таблицы ASCII
        m_cryptedArray = makeAsciiArray(originalPassword);

        // Seed считается как сумма по таблице ASCII.
        int seed = calcSeed(m_cryptedArray);
        setOriginalSeed(seed);  // Сохраняем seed для последующего декодирования

        // Само шифрование осуществляется за счет псевдослучайного swap элементов массива
        for (int i = 0; i < m_cryptedArray.size(); ++i) {
            int indexToSwap = getRand(seed, m_cryptedArray.size());
            std::swap(m_cryptedArray[i], m_cryptedArray[indexToSwap]);
        }
    }
    /*
        Декодируем -> возвращаем массив в исходный порядок
    */
    void decode() {
        int seed = m_originalSeed;  // Восстанавливаем начальный seed

        // Воссоздаем индексы для обратного восстановления порядка
        std::vector<int> swapIndices(m_cryptedArray.size());
        for (int i = 0; i < m_cryptedArray.size(); ++i) {
            swapIndices[i] = getRand(seed, m_cryptedArray.size());
        }

        // Используем воссозданные индексы перестановок в обратном порядке
        for (int i = m_cryptedArray.size() - 1; i >= 0; --i) {
            std::swap(m_cryptedArray[i], m_cryptedArray[swapIndices[i]]);
        }
    }

    /*
        Возвращает расшифрованный пароль как строку
    */
    std::string getDecodedPassword() {
        decode();  // Применяем обратные перестановки

        // Конвертируем ASCII коды обратно в символы строки
        std::string decodedPassword;
        for (int val : m_cryptedArray) {
            decodedPassword += static_cast<char>(val);
        }
        return decodedPassword;
    }
    /*
        для вывода зашифрованного массива
    */
    void print() {
        for (int elem : m_cryptedArray) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }

private:
    std::vector<int> makeAsciiArray(const std::string& password) {
        std::vector<int> asciiArray(password.length());
        for (int i = 0; i < password.length(); ++i) {
            asciiArray[i] = static_cast<int>(password[i]);
        }
        return asciiArray;
    }

    int calcSeed(const std::vector<int>& asciiArray) {
        int seed = 0;
        for (int val : asciiArray) {
            seed += val;
        }
        return seed;
    }

    int getRand(int& seed, int size) {
        seed = (seed * a + c) % size;
        return seed;
    }

private:
    std::string m_password;
    int m_originalSeed;
    std::vector<int> m_cryptedArray;
};

int main() {
    PasswordEncryptor password_1;
    password_1.code("Martin-Iden");

    // Вывод зашифрованного массива
    password_1.print();

    // Декод зашифрованного пароля
    std::string decodedPassword = password_1.getDecodedPassword();
    std::cout << "Decoded Password: " << decodedPassword;

    return 0;
}
