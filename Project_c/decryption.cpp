#include "decryption.h"

char *decryCaesar(int key, string st, char temp[500]){
    char ch;
    int k, flag = 0;
    unsigned int j;
    for (j = 0; j < st.length(); j++) {
        if (st[j] >= 'a'&&st[j] <= 'z')
            k = 'a' + (st[j] - 'a' - key + 26) % 26;
        else
            k = 'A' + (st[j] - 'A' - key + 26) % 26;
        ch = k;
        temp[flag] = ch;
        flag++;
    }
    return temp;
}


char *decryKeyword(string keyword, string st, char temp[500]){
    unsigned i, j;
    int k = 0, n = 0, flag = 0;
    string alpha("abcdefghijklmnopqrstuvwxyz");
    string key(20,'\0');
    string rule(50, '\0');
    for (i = 0; i < keyword.length(); i++) {		//提取key
        flag = 1;
        for (j = 0; j < i; j++) {
            if (key[j] == keyword[i])
                flag = 0;
        }
        if (flag)
            key[k++] = keyword[i];
    }
    key[k] = '\0';		//组合对应表
    while (key[n] != '\0') {
        rule[n] = key[n];
        n++;
    }
    rule[n] = '\0';

    for (i = 0; i < alpha.length(); i++) {
        flag = 1;
        for (j = 0; j < key.length(); j++) {
            if (alpha[i] == key[j]) {
                flag = 0;
                break;
            }
        }
        if (flag)
            rule[n++] = alpha[i];
    }
    rule[n] = '\0';
    for (i = 0; i < st.length(); i++) {
        if (st[i] >= 'a'&&st[i] <= 'z') {
            //cout << rule[(int)(st[i] - 'a')];
            temp[i] = rule[(int)(st[i] - 'a')];
        }
        else {
            temp[i] = rule[(int)(st[i] - 'A')];
            //cout << rule[(int)(st[i] - 'A')];
        }
    }
    return temp;
}

char *decryVigenere(string key, string st, char temp[500]){
    int m, n, k, flag = 0;
    unsigned int i = 0, j = 0;
    char ch;
    char plain;
   // char(*list)[26];
    char list[26][26] = { 0 };
    string rule(50, '\0');

    for (i = 0; i < 26; i++) {
        for (j = 0; j < 26; j++) {
            k = 'a' + (i + j) % 26;
            ch = k;
            list[i][j] = ch;
        }
    }

    j = 0;
    for (i = 0; i < st.length(); i++) {
        j = j % key.length();
        rule[i] = key[j];
        j++;
    }
    rule[i] = '\0';

    //以下是解密关键代码
    for (i = 0; i < st.length(); i++) {
        m = rule[i] - 'a';
        for (j = 0; j < 26; j++) {
            if (list[j][m] == st[i])
                break;
        }
        n = 'a' + j;
        plain = n;
        temp[flag] = plain;
        flag++;
    }

    return temp;
}

char *decryAutokey(string key, string st, char temp[500]){
    int m, n, k;
    int flag = 0;
    char ch;
    char plain;
    unsigned int i, j;
    string rule(50, '\0');
    char list[26][26] = { 0 };

    for (i = 0; i < 26; i++) {
        for (j = 0; j < 26; j++) {
            k = 'a' + (i + j) % 26;
            ch = k;
            list[i][j] = ch;
        }
    }

    j = 0;
    for (i = 0; i < key.length(); i++) {
        j = j % key.length();
        rule[i] = key[j];
        j++;
    }
    rule[i] = '\0';
    for (i = 0; i < st.length() - key.length(); i++) {
       rule[i + key.length()] = st[i];
    }
    rule[i + key.length()] = '\0';
    rule[i] = '\0';

    //以下是解密关键代码
    for (i = 0; i < st.length(); i++) {
        m = rule[i] - 'a';
        for (j = 0; j < 26; j++) {
            if (list[j][m] == st[i])
                break;
        }
        n = 'a' + j;
        plain = n;
        temp[flag] = plain;
        flag++;
    }
    return temp;
}
char* decryAutokey2(string key, string st, char temp[500]){
    unsigned int i, j;
    int m, n, k;
    string rule(50, '\0');
    char ch;
    char list[26][26] = { 0 };

    //构建Vigenere Table
    for (i = 0; i < 26; i++) {
        for (j = 0; j < 26; j++) {
            k = 'a' + (i + j) % 26;
            ch = k;
            list[i][j] = ch;
        }
    }


    //解密与对应关系同时进行
    for (i = 0; i < key.length(); i++) {
        rule[i] = key[i];
    }
    rule[i] = '\0';
    for (i = 0; i < st.length(); i++) {
        m = rule[i] - 'a';
        for (j = 0; j < 26; j++) {
            if (list[j][m] == st[i])
                break;
        }
        n = 'a' + j;
        temp[i] = n;
        if (i < st.length() - key.length()) {
            rule[i + key.length()] = n;
            rule[i + key.length() + 1] = '\0';
        }
    }
    return temp;

}

//解密和加密基本相同，只是修改解密关键代码
char* decryPlayfair(string keyword, string st, char temp[500]){
    unsigned int i = 0, j = 0, flag = 0;
    int k = 0, n = 0, m = 0;
    int num = 0;
    int flag1, flag2, flag3, flag4;
    char matrix[5][5] = { 0 };
    char plain[20][2] = { 0 };
    char tem[50] = { 0 };
    string key(20, '\0');
    string rule(50, '\0');
    string alpha("abcdefghijklmnopqrstuvwxyz");

    //Playfair cipher 处理输入的明文/密文，将格式一致化
    //两两组合配对，检查每组元素是否相同，若相同，则添加元素'q'
    for (i = 0; i < st.length(); i++) {
        if (st[i] == 'j') {		//字母j用字母i替代
            tem[i] = 'i';
            continue;
        }
        tem[i] = st[i];
    }
    tem[i] = '\0';

    for (i = 0; i < strlen(tem) - 1; i++) {
        if ((i % 2 == 0) && (tem[i] == tem[i + 1])) {   //检测分组元素
            for (j = strlen(tem) - 1; j >= i; j--) {
                tem[j + 1] = tem[j];
            }
            tem[i + 1] = 'q';
        }
    }
    //若长度为奇数，则在末尾添加元素'q'，使其成为偶数
    if ((strlen(tem) % 2 != 0) && (tem[strlen(tem) - 1] == 'q')) {
        //cout << "Error!Please choose another encryption algorithm!" << endl;
        exit(-1);
    }
    if ((strlen(tem) % 2 != 0) && (tem[strlen(tem) - 1] != 'q')) {
        tem[strlen(tem)] = 'q';
    }
    tem[strlen(tem)] = '\0';



    //Playfair cipher构造matrix key
    k = 0;
    n = 0;
    flag = 0;
    for (i = 0; i < keyword.length(); i++) {		//提取key
        flag = 1;
        if (keyword[i] == 'j')
            keyword[i] = i;
        for (j = 0; j < i; j++) {
            if (key[j] == keyword[i])
                flag = 0;
        }
        if (flag)
            key[k++] = keyword[i];
    }
    key[k] = '\0';		//组合对应表
    while (key[n] != '\0') {
        rule[n] = key[n];
        n++;
    }
    rule[n] = '\0';

    for (i = 0; i < alpha.length(); i++) {
        flag = 1;
        for (j = 0; j < key.length(); j++) {
            if (alpha[i] == key[j] || alpha[i] == 'j') {   //i,j占据同一个字符
                flag = 0;
                break;
            }
        }
        if (flag)
            rule[n++] = alpha[i];
    }
    rule[n] = '\0';
    // 组建 matrix key 并打印到屏幕
    k = 0;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            matrix[i][j] = rule[k];
            k++;
        }
    }

    k = 0, n = 0, m = 0, flag = 0;
    //将一维数组转化为二维数组，便于加密
    for (i = 0; i < strlen(tem) / 2; i++) {
        for (j = 0; j < 2; j++) {
            plain[i][j] = tem[k];
            k++;
        }
    }

    //以下为解密
    flag = i;
    for (i = 0; i < flag; i++) {
        j = 0;
        for (m = 0; m < 5; m++) {
            for (n = 0; n < 5; n++) {
                j = 0;
                if (matrix[m][n] == plain[i][j]) {
                    flag1 = m;
                    flag2 = n;
                }
                if (matrix[m][n] == plain[i][(j + 1)]) {
                    flag3 = m;
                    flag4 = n;
                }
            }
        }
        //解密关键代码
        if (flag1 == flag3) {
            temp[num++] = matrix[flag1][((flag2 + 4) % 5)];
            temp[num++] = matrix[flag3][((flag4 + 4) % 5)];
            //cout << matrix[flag1][((flag2 + 4) % 5)] << matrix[flag3][((flag4 + 4) % 5)];
        }
        else if (flag2 == flag4) {
            temp[num++] = matrix[((flag1 + 4) % 5)][flag2];
            temp[num++] = matrix[((flag3 + 4) % 5)][flag4];
            //cout << matrix[((flag1 + 4) % 5)][flag2] << matrix[((flag3 + 4) % 5)][flag4];
        }
        else {
            temp[num++] = matrix[flag1][flag4];
            temp[num++] = matrix[flag3][flag2];
            //cout << matrix[flag1][flag4] << matrix[flag3][flag2];
        }
    }
    return temp;

}

char* decryPermutation(string key, string st, char temp[500]){
    unsigned int i, j;
    int flag = 0;
    int k[20] = { 0 };
    int *p;

    p = wordKey(key, k);

    for (i = 0; i < (st.length() / key.length()); i++) {
        for (j = 0; j < key.length(); j++) {
            temp[flag++] = st[(i*key.length() + p[j] - 1)];
        }
    }
    return temp;

}

char* decryColumn(string key, string st, char temp[500]){
    int flag = 0;
    int k[20] = { 0 };
    unsigned int  i, j, m, n;
    char plain[10][10] = { 0 };
    int *p;

    p = wordKey(key, k);

    //组建明文序列

    m = 0;
    n = (st.length()) / (key.length());
    for (i = 0; i < key.length(); i++) {

        for (j = 0; j < n; j++) {
            plain[j][(p[i] - 1)] = st[m];
            m++;
        }
    }

    //输出明文
    for (i = 0; i < n; i++) {
        for (j = 0; j < key.length(); j++) {
            temp[flag++] = plain[i][j];
        }
    }
    return temp;
}
