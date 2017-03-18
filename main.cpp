#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cmath>

#include "andAlgo.h"
#include "andCheck.h"
#include "andUtils.h"

using namespace std;

int main()
{
    const int maxWordLength = 100;
    
    bool checkPalindromes = true;
    bool checkSquares = false;
    cout << "checkPalindromes = " << checkPalindromes << endl;
    
    checkCirc(6, 16, checkSquares, checkPalindromes);
    
    //checkLinear(4, 30);
    
    //checkAtRandom(100, 10000, checkPalindromes);
    // 10^9 ~ 2 sec
    
    return 0;
}

