//
// Created by liududu on 18-10-23.
//

#ifndef CURVEPROJECT_HELPER_H
#define CURVEPROJECT_HELPER_H

#include <vector>
#include <iostream>
using namespace std;

//阶乘
unsigned int fact (unsigned int n)
{
    unsigned int res;
    static vector<int> Fact;      //Fact = new int[…];
    static unsigned int num = 0;
    if (n <= num)   return n == 0?1:Fact[n-1];
    else {
        if(n == 1 || n == 0)      res = 1;
        else               res =   n*fact(n-1);
        num = n;
        Fact.push_back(res);
        return res;
    }
}

//组合数
unsigned int C(int m, int n) {
    if (n == 0) return 0;
    return fact(n)/ (fact(n - m) * fact(m));
}
#endif //CURVEPROJECT_HELPER_H
