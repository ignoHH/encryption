#ifndef DH_FUN_H
#define DH_FUN_H
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <string>
#include "big_int.h"
#include "table.h"
#include "string.h"

#define MAX 500 //运算中的最高可能位数
#define NUMG 50 //随机数g的位数
#define swap(x,y) x^=y^=x^=y

using namespace std;


//检测素数
long remainder(big_int &a, unsigned long b)
{
    return a%b;
}

int Rabin_miller(big_int &x)
{
    for (int i = 0; i<1000; i++) { if (remainder(x, table[i]) == 0) return 0; }
    big_int s, a, r, z;
    z = x;
    z.value[0]--;
    for (int i = 0; i<5; i++)
    {
        a = rand()*rand();
        s = z / 2;
        r = a.power_mod(s, x);
        if (r != 1 && r != z)
            return 0;
    }
    return 1;
}

//产生大素数
big_int prime() {
    big_int x;

begin:
    for (int i = 0; i<16; i++)
        x.value[i] = (rand() << 17) + (rand() << 1) + rand() % 2;
    x.value[0] = x.value[0] | 1;
    x.length = 16;

    if (!Rabin_miller(x))
        goto begin;
    return x;
}

//输出结果
void out(big_int &x, char *str) {
    int length = 0;
    if ((x.length == 1) && (x.value[0] == 0)) { str[0] = '0'; str[1] = '\0'; return; }
    char* t = "0123456789ABCDEF";
    int a;
    char ch;
    big_int u;
    u = x;
    while (u.value[u.length - 1]>0)
    {
        length++;
        u = u / 16;
    }
    str[length] = '\0';
    u = x;
    while (u.value[u.length - 1]>0)
    {
        a = u % 16;
        ch = t[a];
        str[--length] = ch;
        u = u / 16;
    }
}

void generate(big_int &big_p, big_int &bg){
    char ch[500] = "";
    srand(time(NULL));
    cout << "p - a large prime number" << endl;
    cout << "g - a random number less than p" << endl;
    cout << "a - a secret random number choosed by Alice" << endl;
    cout << "b - a secret random number choosed by BOB" << endl << endl;

    cout << "Running to generate a big prime..." << endl;
    big_p = prime();
    cout << "p = ";
    out(big_p, ch);
    puts(ch);

    cout << endl;
    cout << "Running to generate a random number g..." << endl;
    bg = rand();
    while (bg >= big_p)
        bg = rand();
    cout << "g = ";
    cout << bg.value[0] << endl;

    cout << endl << "a = ";
}

void send(big_int &AB, big_int &AB_send, string st, big_int &big_p, big_int &bg){
    int i, j;
    int test_flag = 0;
    char a[500] = {0};
    char ch[500] = {0};
    for(i = 0; i<st.length(); i++){
        a[i] = st[i];
    }

    for (i = 0; i<32; i++)
    {
        for (j = 0; j<4; j++)
        {
            if (a[test_flag] == '\n')
                break;
            else
            {
                if (j == 0)
                    AB.value[i] = (unsigned long)a[test_flag++];
                else
                    AB.value[i] = AB.value[i] + (unsigned long)(a[test_flag++] << (8 * j));
            }
        }
        if (a[test_flag] == '\n')
        {
            if (j == 0)
                i--;
            break;
        }
    }

    AB.length = ++i;
    AB_send = bg.power_mod(AB, big_p);

    out(AB_send, ch);
    cout << "Alice send: ";
    puts(ch);
}

caculate(big_int &K, big_int &B_send, big_int &A, big_int &big_p){
    char ch[500] = "";
    K = B_send.power_mod(A, big_p);
    cout << endl;
    out(K, ch);
    puts(ch);
}

#endif // DH_FUN_H
