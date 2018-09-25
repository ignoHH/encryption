#include "encryption.h"

char *encryCaesar(int key, string st, char temp[500]){
    char ch;
    int k, flag = 0;
    unsigned int j;
    for (j = 0; j < st.length(); j++) {
        if (st[j] >= 'a'&&st[j] <= 'z')
            k = 'a' + (st[j] - 'a' + key) % 26;
        else
            k = 'A' + (st[j] - 'A' + key) % 26;
        ch = k;
        temp[flag] = ch;
        flag++;
    }
    return temp;
}

char *encryKeyword(string keyword, string st, char temp[500]){
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

char *encryVigenere(string key, string st, char temp[500]){
    int m, n, k, flag = 0;
    unsigned int i = 0, j = 0;
    char ch;
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
    for (i = 0; i < st.length(); i++) {
        m = rule[i] - 'a';
        n = st[i] - 'a';
        temp[flag] = list[n][m];
        flag++;
    }
    return temp;
}

char *encryAutokey(string key, string st, char temp[500]){
    int m, n, k;
    int flag = 0;
    unsigned int i = 0, j = 0;
    char ch;
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

        m = rule[i] - 'a';
        n = st[i] - 'a';
        rule[i + key.length()] = list[n][m];

    }
    rule[i + key.length()] = '\0';
    for (i = 0; i < st.length(); i++) {
        m = rule[i] - 'a';
        n = st[i] - 'a';
        temp[flag] = list[n][m];
        flag++;
    }
    return temp;
}

char *encryAutokey2(string key, string st, char temp[500]){
    unsigned int i, j;
    int m, n, k;
    int flag = 0;
    char ch;
    string rule(50, '\0');
    char list[26][26] = { 0 };

    //构建Vigenere Table
    for (i = 0; i < 26; i++) {
        for (j = 0; j < 26; j++) {
            k = 'a' + (i + j) % 26;
            ch = k;
            list[i][j] = ch;
        }
    }

    for (i = 0; i < key.length(); i++) {
        rule[i] = key[i];
    }
    rule[i] = '\0';
    for (i = 0; i < st.length() - key.length(); i++) {
        rule[i + key.length()] = st[i];

    }
    rule[i + key.length()] = '\0';

    for (i = 0; i < st.length(); i++) {
        m = rule[i] - 'a';
        n = st[i] - 'a';
        temp[flag] = list[n][m];
        flag++;
    }
    return temp;
}

char* encryPlayfair(string keyword, string st, char temp[500]){
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

    //以下为加密
    flag = i;
    //cout << "CipherText:";
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
        if (flag1 == flag3) {
           // cout << matrix[flag1][((flag2 + 1) % 5)] << matrix[flag3][((flag4 + 1) % 5)];
            temp[num++] = matrix[flag1][((flag2 + 1) % 5)];
            temp[num++] = matrix[flag3][((flag4 + 1) % 5)];
        }
        else if (flag2 == flag4) {
            temp[num++] = matrix[((flag1 + 1) % 5)][flag2];
            temp[num++] = matrix[((flag3 + 1) % 5)][flag4];
          //  cout << matrix[((flag1 + 1) % 5)][flag2] << matrix[((flag3 + 1) % 5)][flag4];
        }
        else {
            temp[num++] = matrix[flag1][flag4];
            temp[num++] = matrix[flag3][flag2];
            //cout << matrix[flag1][flag4] << matrix[flag3][flag2];
        }
    }
    return temp;

}

char* encryPermutation(string key, string st, char temp[500]){
    int flag = 0;
    unsigned int i, j, t;
    int k[20] = { 0 };
    int *p;
    char plain[30] = { 0 };
    char cipher[30] = { 0 };

    //明文不是key的整数倍补'x'
    t = st.length();
    if (t%key.length() != 0)
        t = ((t / (key.length()) + 1)*key.length());
    for (i = 0; i < t; i++) {
        if (i < st.length())
            plain[i] = st[i];
        else
            plain[i] = 'x';
    }
    p = wordKey(key, k);


    //整理密文
    for (i = 0; i < t / (key.length()); i++) {
        for (j = 0; j < key.length(); j++) {
            cipher[i*key.length() + p[j] - 1] = plain[i*key.length() + j];
        }
    }

    //输出密文
    for (i = 0; i < strlen(cipher); i++) {
        temp[flag++] = cipher[i];
    }

    return temp;
}

char *encryColumn(string key, string st, char temp[500]){
    int m, flag = 0;
    int k[20] = { 0 };
    unsigned int i, j, t, n;
    char tem[50] = { 0 };
    char cipher[10][10] = { 0 };
    int *p;

    //明文不是key的整数倍补'x'
    t = st.length();
    if (t%key.length() != 0)
        t = ((t / (key.length()) + 1)*key.length());
    for (i = 0; i < t; i++) {
        if (i < st.length())
            tem[i] = st[i];
        else
            tem[i] = 'x';
    }

    //组建plain matrix
    m = 0;
    n = (strlen(tem)) / (key.length());
    for (i = 0; i < n; i++) {

        for (j = 0; j < key.length(); j++) {
            cipher[i][j] = tem[m];
            m++;
        }
    }
    p = wordKey(key, k);
    //加密
    for (i = 0; i < key.length(); i++) {
        for (j = 0; j < n; j++) {
            temp[flag++] = cipher[j][(p[i] - 1)];
        }
    }
    return temp;

}
