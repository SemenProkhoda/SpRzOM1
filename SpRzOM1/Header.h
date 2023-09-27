#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

using namespace std;
//http://mindhalls.ru/big-number-in-c-cpp-add-sub/
//Деление для отрицательных чисел
int my_div(int num, int diver) {
    if ((num < 0) && (num % diver))
        return num / diver - 1;
    else
        return num / diver;
}

//Взятие по модулю для отрицательных чисел
int my_mod(int num, int diver) {
    if ((num < 0) && (num % diver))
        return num % diver + diver;
    else
        return num % diver;
}

//Класс "большое число", описывает способ хранения большого числа и
//длинную арифметику
class BigNumber {
private:
    vector<int> chunks;
    int sign;

    //Настроиваемые параметры хранения большого числа
    //Количество символов в строке, которые станут одной "чанкой"
    static const int BASE = 2;
    //Зависит от BASE (BASE10 = 10^BASE), используется при нормализации
    static const int BASE10 = 100;

    BigNumber _plus(BigNumber& a);
    BigNumber _minus(BigNumber& a);
    void _normalizationSigns();
    void _normalizationChunks();
    void _resize(int newsize);

public:
    BigNumber operator + (BigNumber& num);
    BigNumber operator - (BigNumber& num);

    friend ostream& operator << (ostream& os, BigNumber& num);

    int getBASE() {
        return this->BASE;
    }

    //Конструктор, строку конвертирует в большое число
    BigNumber(string str) {
        int i;
        if (BASE != 1) {
            //Записываем с конца по BASE символов строки в чанки
            for (i = str.size() - BASE; i >= BASE - 1; i -= BASE) {
                chunks.push_back(stoi(str.substr(i, BASE)));
            }
        }
        else {
            for (i = str.size() - BASE; i >= BASE; i -= BASE) {
                chunks.push_back(stoi(str.substr(i, BASE)));
            }
        }

        //Дошли до начала строки, тут ищем знак и записываем последнюю чанку
        if (str[0] == '-') {
            sign = -1;
            if (i + BASE - 1 != 0) {
                chunks.push_back(stoi(str.substr(1, i + BASE - 1)));
            }
        }
        else {
            sign = 1;
            chunks.push_back(stoi(str.substr(0, i + BASE)));
        }
    }

    //Конструктор без аргументов - "пустое" положительное число
    BigNumber() {
        sign = 1;
    }
};

//Изменение размера массива с чанками
void BigNumber::_resize(int newSize) {
    chunks.resize(newSize);
}

/*
 * Функция нормализует большое число так, чтобы
 * во всех чанках лежали BASE-разрядные числа
*/
void BigNumber::_normalizationChunks() {
    int over = 0; //"Лишнее", которое будет кочевать в следующие чанки
    for (int i = 0; i < chunks.size() - 1; i++) {
        chunks[i] += over; //Прибавляем привесок от прошлых чанок
        over = my_div(chunks[i], BASE10); //Считаем перевес в текущей чанке
        chunks[i] = my_mod(chunks[i], BASE10); //Выравниваем чанку по базе
    }

    //Прибавляем перевес к последней чанке
    chunks[chunks.size() - 1] += over;
    //Обрабатываем перевес в последней чанке
    if (chunks[chunks.size() - 1] / BASE10) {
        over = my_div(chunks[chunks.size() - 1], BASE10);
        chunks[chunks.size() - 1] = my_mod(chunks[chunks.size() - 1], BASE10);
        chunks.push_back(over); //Создаем нову чанку с остатками
    }

    return;
}

//Функция нормализует большое число для печати так,
//чтобы все чанки были положительными, но sign = -1(если число отрицательное)
void BigNumber::_normalizationSigns() {
    //Если в последней чанке отрицательное число
    if (chunks[chunks.size() - 1] < 0) {
        sign = -sign; //Меняем знак числа
        chunks[0] = BASE10 - chunks[0]; //Нормализуем первую чанку
        for (int i = 1; i < chunks.size(); i++) {
            chunks[i] = (BASE10 - chunks[i] - 1) % BASE10; //Нормализуем ост. чанки
        }
    }

    //Убираем из числа нулевые чанки
    int i = chunks.size() - 1;
    while (chunks[i] == 0) {
        if (i == 0) {
            sign = 1;
            return;
        }

        chunks.pop_back();
        i--;
    }

    return;
}

//Функция сложения
BigNumber BigNumber::_plus(BigNumber& num) {
    BigNumber res;
    res.sign = this->sign;

    for (int i = 0; i < this->chunks.size(); i++) {
        res.chunks.push_back(this->chunks[i] + num.chunks[i]);
    }

    return res;
}

//Функция вычитания
BigNumber BigNumber::_minus(BigNumber& num) {
    BigNumber res;
    res.sign = this->sign;

    for (int i = 0; i < this->chunks.size(); i++) {
        res.chunks.push_back(this->chunks[i] - num.chunks[i]);
    }

    return res;
}

//Оператор +, выполняет корректное сложение больших чисел
BigNumber BigNumber::operator + (BigNumber& num) {
    BigNumber res;

    //Приводим размер чанок обоих чисел
    if (this->chunks.size() > num.chunks.size()) {
        num._resize(chunks.size());
    }
    else {
        (*this)._resize(num.chunks.size());
    }

    //Выполняем операцию в зависимости от знаков чисел
    if (sign == num.sign) {
        res = (*this)._plus(num);
    }
    else {
        res = (*this)._minus(num);
    }

    //Нормализуем результат
    res._normalizationChunks();
    return res;
}

//Оператор -, выполняет корректное вычитание
BigNumber BigNumber::operator - (BigNumber& num) {
    BigNumber res;

    //Приводим размер чанок
    if (this->chunks.size() > num.chunks.size()) {
        num._resize(chunks.size());
    }
    else {
        (*this)._resize(num.chunks.size());
    }

    //В зависимости от знаков, выполняем нужное действие
    if (sign != num.sign) {
        res = (*this)._plus(num);
    }
    else {
        res = (*this)._minus(num);
    }

    //Нормализуем результат
    res._normalizationChunks();
    return res;
}

//Перегрузка оператора << для вывода в поток
ostream& operator << (ostream& os, BigNumber& num) {
    num._normalizationSigns();

    if (num.sign == -1) {
        os << '-';
    }

    os << num.chunks[num.chunks.size() - 1];

    for (int i = num.chunks.size() - 2; i >= 0; i--) {
        os << setw(num.getBASE()) << setfill('0') << num.chunks[i];
    }

    return os;
}
