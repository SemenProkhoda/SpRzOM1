#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Large {
private:
    vector<uint64_t> data; // Хранение разрядов числа
    static const uint64_t BASE = 1000000000ULL; // Основание системы счисления (9 десятичных разрядов)
public:
    Large();
    Large(const string& str);
    Large(uint64_t value);

    void fromString(const string& str);
    void fromUInt64(uint64_t value);
    string toString() const;

    Large operator+(const Large& other) const;
    Large operator-(const Large& other) const;
    Large operator*(const Large& other) const;
    Large operator/(const Large& other) const;
    Large operator%(const Large& other) const;
    Large operator^(uint64_t exp) const;
    bool operator==(const Large& other) const;
    bool operator!=(const Large& other) const;
    bool operator<= (const Large & other) const;
    bool operator>=(const Large& other) const;
    bool operator>(const Large& other) const;
    bool operator<(const Large& other) const;
    int highestSetBit() const;
    Large operator<<(int shift) const;
    Large operator>>(int shift) const;

};

inline Large::Large() {
    data.push_back(0);
}

inline Large::Large(const string& str) {
    fromString(str);
}

inline Large::Large(uint64_t value) {
    fromUInt64(value);
}

inline void Large::fromString(const string& str) {
    data.clear();
    int start = 0;
    while (start < str.size() && str[start] == '0') {
        start++;
    }

    if (start == str.size()) {
        data.push_back(0);
        return;
    }

    for (int i = str.size() - 1; i >= start; i -= 9) {
        uint64_t digit = 0;
        for (int j = max(start, i - 8); j <= i; j++) {
            digit = digit * 10 + (str[j] - '0');
        }
        data.push_back(digit);
    }
}

void Large::fromUInt64(uint64_t value) {
    data.clear();
    while (value > 0) {
        data.push_back(value % BASE);
        value /= BASE;
    }
    if (data.empty()) {
        data.push_back(0);
    }
}

inline string Large::toString() const {
    if (data.empty()) {
        return "0";
    }

    string result;
    result += to_string(data.back());

    for (int i = data.size() - 2; i >= 0; i--) {
        string str = to_string(data[i]);
        while (str.size() < 9) {
            str = "0" + str;
        }
        result += str;
    }

    return result;
}

inline Large Large::operator+(const Large& other) const {
    Large result;
    uint64_t carry = 0;

    size_t max_size = max(data.size(), other.data.size());
    result.data.resize(max_size);

    for (size_t i = 0; i < max_size; i++) {
        uint64_t sum = carry;
        if (i < data.size()) {
            sum += data[i];
        }
        if (i < other.data.size()) {
            sum += other.data[i];
        }

        result.data[i] = sum % BASE;
        carry = sum / BASE;
    }

    if (carry != 0) {
        result.data.push_back(carry);
    }

    return result;
}

inline Large Large::operator-(const Large& other) const
{
    Large result;
    int carry = 0;

    size_t max_size = max(data.size(), other.data.size());
    result.data.resize(max_size);

    for (size_t i = 0; i < max_size; i++) {
        int sub = carry;
        if (i < data.size()) {
            sub += data[i];
        }
        if (i < other.data.size()) {
            sub -= other.data[i];
        }

        if (sub < 0) {
            sub += BASE;
            carry = -1;
        }
        else {
            carry = 0;
        }

        result.data[i] = sub;
    }

    while (result.data.size() > 1 && result.data.back() == 0) {
        result.data.pop_back();
    }

    return result;
}

inline Large Large::operator*(const Large& other) const {
    Large result;
    result.data.resize(data.size() + other.data.size());

    for (size_t i = 0; i < data.size(); i++) {
        uint64_t carry = 0;
        for (size_t j = 0; j < other.data.size() || carry; j++) {
            uint64_t product = result.data[i + j] + carry + data[i] * (j < other.data.size() ? other.data[j] : 0);
            result.data[i + j] = product % BASE;
            carry = product / BASE;
        }
    }

    while (result.data.size() > 1 && result.data.back() == 0) {
        result.data.pop_back();
    }

    return result;
}

inline Large Large::operator/(const Large& other) const
{
    if (other == 0) {
        throw std::runtime_error("Division by zero.");
    }

    Large quotient;
    Large remainder = *this;
    Large divisor = other;
    Large factor;

    while (remainder >= divisor) {
        factor = divisor;
        Large temp(1);

        while (remainder >= factor) {
            remainder = remainder - factor;
            quotient = quotient + temp;
            factor = factor << 1;
            temp = temp << 1;
        }
    }

    return quotient;
}

inline Large Large::operator%(const Large& other) const
{
    Large quotient;
    Large remainder = *this;
    Large divisor = other;

    if (other == 0) {
        throw runtime_error("Division by zero.");
    }

    while (remainder >= divisor) {
        int shift = remainder.highestSetBit() - divisor.highestSetBit();
        Large temp = divisor << shift;
        while (remainder >= temp) {
            remainder = remainder - temp;
            quotient = quotient + (Large(1) << shift);
        }
    }

    return remainder;
}

inline Large Large::operator^(uint64_t exp) const
{
    if (exp == 0) {
        return Large(1);
    }

    Large result = *this;
    Large base = *this;
    exp--;

    while (exp > 0) {
        if (exp % 2 == 1) {
            result = result * base;
        }
        base = base * base;
        exp /= 2;
    }

    return result;
}

inline bool Large::operator==(const Large& other) const
{
    return data == other.data;
}

inline bool Large::operator!=(const Large& other) const
{
    return data != other.data;
}

inline bool Large::operator<=(const Large& other) const
{
    return data <= other.data;
}

inline bool Large::operator>=(const Large& other) const
{
    return data >= other.data;
}

inline bool Large::operator>(const Large& other) const
{
    return data > other.data;
}

inline bool Large::operator<(const Large& other) const
{
    return data < other.data;
}

inline int Large::highestSetBit() const
{
    for (int i = data.size() - 1; i >= 0; i--) {
        if (data[i] != 0) {
            for (int j = 31; j >= 0; j--) {
                if ((data[i] >> j) & 1) {
                    return static_cast<int>(i * 32 + j);
                }
            }
        }
    }
    return 0;
}

inline Large Large::operator<<(int shift) const
{
    if (shift < 0) {
        throw invalid_argument("Negative shift value.");
    }

    Large result;
    int shiftDigits = shift / 9;
    int shiftBits = shift % 9;

    result.data.resize(data.size() + shiftDigits + 1, 0);

    for (size_t i = 0; i < data.size(); i++) {
        uint64_t bits = data[i];
        uint64_t lowBits = bits << shiftBits;
        uint64_t carry = lowBits / BASE;
        result.data[i + shiftDigits] += lowBits % BASE;

        for (int j = 1; carry > 0; j++) {
            result.data[i + shiftDigits + j] += carry;
            carry = result.data[i + shiftDigits + j] / BASE;
            result.data[i + shiftDigits + j] %= BASE;
        }
    }

    while (result.data.size() > 1 && result.data.back() == 0) {
        result.data.pop_back();
    }

    return result;
}

inline Large Large::operator>>(int shift) const
{
    if (shift < 0) {
        throw invalid_argument("Negative shift value.");
    }

    Large result;
    int shiftDigits = shift / 9;
    int shiftBits = shift % 9;

    if (shiftDigits >= data.size()) {
        return Large(0);
    }

    result.data.resize(data.size() - shiftDigits, 0);

    for (size_t i = data.size() - 1; i >= shiftDigits; i--) {
        uint64_t bits = data[i];
        uint64_t highBits = bits >> shiftBits;
        uint64_t lowBits = (i > 0) ? (data[i - 1] << (9 - shiftBits)) : 0;
        result.data[i - shiftDigits] = highBits + (lowBits % BASE);
    }

    while (result.data.size() > 1 && result.data.back() == 0) {
        result.data.pop_back();
    }

    return result;
}
