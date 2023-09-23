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
    Large operator*(const Large& other) const;

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
