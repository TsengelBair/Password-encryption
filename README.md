## Шифрование паролей 

*Входные параметры:* строка, которую нужно зашифровать

*Выходные:* зашифрованный пароль в виде массива int


**Этапы работы алгоритма:**

1) Каждый символ переданной строки преобразуется до int значения, соответствующего кодировке в ASCII таблице

![1](/img/1.jpg)

```c++
std::vector<int> makeAsciiArray(const std::string& password) {
    std::vector<int> asciiArray(password.length());
    for (int i = 0; i < password.length(); ++i) {
        asciiArray[i] = static_cast<int>(password[i]);
    }
    return asciiArray;
}
```

2) Генерация исходного seed, который считается как сумма элементов asciiArray

```c++
int seed = calcSeed(m_cryptedArray);
int calcSeed(const std::vector<int>& asciiArray) {
    int seed = 0;
    for (int val : asciiArray) {
        seed += val;
    }
    return seed;
}
```

Причем изначальный seed нужно сохранить для последующего декода, т.к. метод генерирующий рандомный индекс работает с seed по ссылке, поэтому в методе code сохраняем исходный seed 

```c++
setOriginalSeed(seed);
```

3) Непосредственное шифрование осуществляется за счет рандомных перестановок элементов массива asciiArray

```c++
for (int i = 0; i < m_cryptedArray.size(); ++i) {
    int indexToSwap = getRand(seed, m_cryptedArray.size());
    std::swap(m_cryptedArray[i], m_cryptedArray[indexToSwap]);
}
```

Рандомный индекс получаем из кастомной функции псевдорандома, причем сам параметр seed мутируем для последующих перестановок

```c++
int getRand(int& seed, int size) {
    seed = (seed * a + c) % size;
    return seed;
}
```

Таким образом происходит шифрование переданной строки до массива int

### Декод

1) Сбрасываем начальный seed 

```c++
int seed = m_originalSeed;
```

2) Зная начальный seed, можем получить точно такую же последовательность сгенерированных в code() индексов 

```c++
// Воссоздаем индексы 
std::vector<int> swapIndices(m_cryptedArray.size());
for (int i = 0; i < m_cryptedArray.size(); ++i) {
    swapIndices[i] = getRand(seed, m_cryptedArray.size());
}
```

3) Восстанавливаем порядок, "откатывая" назад действия (итерируемся с конца)

```c++
// Используем воссозданные индексы перестановок в обратном порядке
for (int i = m_cryptedArray.size() - 1; i >= 0; --i) {
    std::swap(m_cryptedArray[i], m_cryptedArray[swapIndices[i]]);
}
```

4) Получаем исходный массив и преобразуем int элементы в соответствующие символы 

```c++
// Конвертируем ASCII коды обратно в символы строки
std::string decodedPassword;
for (int val : m_cryptedArray) {
    decodedPassword += static_cast<char>(val);
}
return decodedPassword;
```

### Тестируем

```c++
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
```

![2](/img/2.jpg)

// TODO

Собрать проект под Qt либу 