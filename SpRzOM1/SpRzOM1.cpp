#include <iostream>
#include <vector>
#include "Large.hpp"

using namespace std;

int main() {
    Large num1("9121234541");
    Large num2("2123234541");

    Large sum = num1 + num2;
    cout << "Sum: " << sum.toString() << endl;

    Large difference = num1 - num2;
    cout << "Difference: " << difference.toString() << "\n";

    Large product = num1 * num2;
    cout << "Product: " << product.toString() << "\n";

    Large quotient = num1 / num2;
    cout << "Quotient: " << quotient.toString() << "\n";

    Large remainder = num1 % num2;
    cout << "Remainder: " << remainder.toString() << "\n";

    Large power = num1 ^ 5;
    cout << "Power: " << power.toString() << "\n";

    int highestBit = num1.highestSetBit();
    cout << "Highest Set Bit: " << highestBit << "\n";

    Large leftShift = num1 << 3;
    cout << "Left Shift: " << leftShift.toString() << "\n";

    Large rightShift = num1 >> 3;
    cout << "Right Shift: " << rightShift.toString() << "\n";
    return 0;
}
