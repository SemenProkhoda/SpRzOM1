#include <iostream>
#include <vector>
#include "Large.hpp"
#include "header.h"

using namespace std;

int main() {

    BigNumber n1("200000000000000000000000000000000000000000000000000000000000010");
    BigNumber n2("-20");
    BigNumber n3 = n1 - n2;
    cout << n3 << endl;
    return 0;

}
