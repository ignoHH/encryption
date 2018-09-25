#ifndef FUN_DH_SIMPLE_H
#define FUN_DH_SIMPLE_H

#include <iostream>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include <string>
using namespace std;

bool witness(long int a, long long n)
{
    //得到n-1的二进制表示
    int div = (int)n;
    int b[128];
    int k = 0;
    int i;
    while (div != 0)
    {
        b[k] = div % 2;
        div = div / 2;
        k++;
    }
    //计算
    int x;
    int d = 1;
    for (i = k - 1; i >= 0; i--)
    {
        x = d;
        d = (d*d) % n;
        if (d == 1 && x != 1 && x != n - 1)
            return true;
        if (b[i] == 1)
            d = (d*a) % n;
    }
    if (d != 1)
        return true;
    else
        return false;

}

void fun_pri(long int &digit, long int &g) {
    //long digit;
    long int a;
    long int i = 0;
    long int j;
    srand((unsigned)time(NULL));   //以时间为获得产生随机数的种子
    while (1)
    {
        digit = rand();
        while (digit % 2 == 0)
            digit = rand();       //随机产生一个奇数
        //cout << "the cow prime is " << digit << "\n";
              i=0;
        while(i<digit)
        {
        //a为小于digit的随机数
        a=(int)(digit/(float)RAND_MAX * rand());
    //	cout<<a<<"\n";
        //素数测试（Miller_Rabin测试法），返回true表示不是素数
        if(witness(a,digit))  break;
        i++;
        }
            for (i = 2; i <= sqrt(digit); i++)
        {
            if (digit%i == 0)  break;
        }

    //	cout << "i=" << i << "\n";

        if (i + 1>(int)sqrt(digit))
        {
            //cout << "digit " << digit << " is big prime!\n";
            break;
        }
    }
    g = rand();
    while (g > digit)
        g = rand();
    //cout << endl << "g = " << g << endl;

}

long sim_caculate(long int m, long int e, long int n)
{
    long t = 1;
    long i;
    for (i = 1; i <= e; i++)
    {
        t *= m;
        t = t%n;
    }
    return t%n;
}

char* encryCaesar_simple(long int b, string st, char temp[100]){
    int j, k;
    int flag = 0;
    char ch;
    //char temp[100] = {0};
    cout << "-------------Caesar cipher(Encryption)------------" << endl;
    cout << "Please enter the plaintext:" << endl;

    b = b % 26;
    cout << "key = ";
    cout << b;
    cout << endl;

    cout << "Ciphertext: ";
    for (j = 0; j < st.length(); j++) {
        if (st[j] >= 'a'&&st[j] <= 'z')
            k = 'a' + (st[j] - 'a' + b) % 26;
        else
            k = 'A' + (st[j] - 'A' + b) % 26;
        ch = k;
        temp[flag++] = ch;
        cout << ch;
    }
    return temp;
}

char* decryCaesar_simple(long int b, string st, char temp[100]){
    int j, k;
    int flag = 0;
    char ch;
    b = b % 26;
    cout << "key = ";
    cout << b;
    cout << endl;
    cout << "Ciphertext: ";
    for (j = 0; j < st.length(); j++) {
        if (st[j] >= 'a'&&st[j] <= 'z')
            k = 'a' + (st[j] - 'a' - b + 26) % 26;
        else
            k = 'A' + (st[j] - 'A' - b + 26) % 26;
        ch = k;
        temp[flag++] = ch;
        cout << ch;
    }

    return temp;
}

#endif // FUN_DH_SIMPLE_H
