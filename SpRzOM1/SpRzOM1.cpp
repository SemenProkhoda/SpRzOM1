#include <iostream>
#include <vector>
#include "Large.hpp"

using namespace std;

int main() {
    Large num1("9823748905723984750982347597812340875442");
    Large num2("7189236597231846102983461283649872630212");

    Large sum = num1 + num2;
    cout << "Sum: " << sum.toString() << endl;

    Large product = num1 * num2;
    cout << "Product: " << product.toString() << endl;

    return 0;
}
