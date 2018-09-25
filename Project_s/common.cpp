#include "common.h"

int *wordKey(string key, int k[20]) {
    unsigned int i, j;
    //	int k[20] = { 0 };
    int *p = k;
    int flag = 0;
    char temp1;
    char tem[20] = { 0 };
    char keygroup[20] = { 0 };

    for (i = 0; i < key.length(); i++) {//设置临时数组
        keygroup[i] = key[i];
    }
    //冒泡排序
    for (i = 0; i < key.length(); i++) {
        flag = 0;
        for (j = 0; j < key.length() - i - 1; j++) {
            if (keygroup[j] > keygroup[j + 1]) {
                temp1 = keygroup[j];
                keygroup[j] = keygroup[j + 1];
                keygroup[j + 1] = temp1;
                flag = 1;
            }
        }
        if (flag == 0)
            break;
    }

    //设置临时数组
    for (i = 0; i < key.length(); i++) {

        tem[i] = keygroup[i];
    }


    for (i = 0; i < key.length(); i++) {
        for (j = 0; j < key.length(); j++) {
            if (keygroup[j] == key[i] && tem[j] != '#') {
                k[i] = j + 1;
                tem[j] = '#';
                break;
            }

        }
    }
    return p;

}
