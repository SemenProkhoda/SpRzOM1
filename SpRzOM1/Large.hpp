#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Large {
private:
    vector<int> nums;
    bool isNegative;

public:
    // Конструктори
    Large();
    Large(int num);
    Large(string& str);
    string toDecimalString();
    string toHexString();
    string toBinaryString();
    Large operator+(Large& other);
    Large operator-(Large& other);
    Large operator*(Large& other);
    Large operator^(int exponent);
    Large operator/(Large& other);
    Large operator%(Large& other);
    bool operator==(Large& other);
    bool operator!=(Large& other);
    bool operator<(Large& other);
    bool operator<=(Large& other);
    bool operator>(Large& other);
    bool operator>=(Large& other);
    friend ostream& operator<<(ostream& os, Large& num);
    friend istream& operator>>(istream& is, Large& num);
private:
    void removeLeadingZeros();
    void negate();
};