#include "des.h"

int Check(string s) {
    int i = 0;
    while (s[i] != '\0') {
        if (!((s[i] >= 'a'&&s[i] <= 'z') || (s[i] >= 'A'&&s[i] <= 'Z'))) {
            cout << "Error!Please enter again!" << endl;
            Sleep(1000);
            return 1;
        }
        i++;
    }
    return 0;
}

int *keyTrans(string key, int keytemp[64]) {	//密钥二进制转换并输出
    int i, k, t, m, n, q;
    int block;
    int flag = 0;
    int temp[56] = { 0 };
    int *p = keytemp;
    m = 0;
    for (k = 0; k < 7; k++) {
        t = key[6 - k];		//二进制转换
        for (i = 0; i < 8; i++) {

            temp[56 - 1 - m] = t % 2;
            t /= 2;
            m++;
        }
    }
    cout << endl;

    cout << "-----------------------------------Key Information----------------------------------" << endl << endl;
    cout << "Key(56bit): ";		//56位密钥
    for (i = 0; i < 56; i++) {
        cout << temp[i];
        if ((i + 1) % 7 == 0)
            cout << " ";
    }
    cout << endl << endl;

    n = 0;
    m = 0;
    q = 0;
    cout << "Key(64bit)(With odd parity)" << endl;	//	显示有奇偶校验位的64位密钥
    cout << "Key(64bit): ";
    for (i = 0; i < 56; i++) {
        cout << temp[i];
        keytemp[q] = temp[i];
        q++;
        if (temp[i] == 0) {
            n++;
        }

        if (((m + 1) % 7 == 0) && (n % 2 != 0)) {
            cout << "1 ";
            keytemp[q] = 1;	//将64位密钥存入数组
            q++;
            m++;
            n = 0;
            continue;
        }
        else if (((m + 1) % 7 == 0) && (n % 2 == 0)) {
            cout << "0 ";
            keytemp[q] = 0;
            q++;
            m++;
            n = 0;
            continue;
        }
        m++;
    }
    cout << endl << endl;
    return p;
}

void plainTrans(string st, int block, int(&plaintext)[800][64], int way,char streamPlain[500]) {	//	实现明文/密文分块和二进制转化
    unsigned int i, k, t, m, n, j;
    int flag = 0;
    int num = 0;
    int temp[56] = { 0 };

    //将明文分块，存入二维数组
    n = 0;
    k = st.length() % 8;
    if (k == 0)
        k = 8;
    m = block;
    for (i = 0; i < st.length(); i++) {
        t = (unsigned int)st[(st.length() - 1 - i)];
        if (t > 256) {
            t = t - 4294967040;		//若字符的ASCII超出范围，则减去过余码
        }
        for (j = 0; j < 8; j++) {
            if (n == 64) {
                n = 0;
                m--;
            }
            if (m != block) {
                plaintext[m][(64 - 1 - n)] = t % 2;
            }
            else {
                plaintext[m][(8 * k - 1 - n)] = t % 2;
                if (n == (8 * k - 1)) {	//明文最后一个分块，不足64位补0
                    n = -1;
                    m--;
                }
            }
            t /= 2;
            n++;
        }
    }
    cout << endl;

    //输出分块明文比特流
    if (way == 0) {
        cout << "--------------------------------Binary Plaintext Block-------------------------------" << endl << endl;
    }
    else {
        cout << "--------------------------------Binary Ciphertext Block------------------------------" << endl << endl;
    }

    for (i = 0; i < block + 1; i++) {
        cout << "Block" << i + 1 << ":";
        for (j = 0; j < 64; j++) {
            if(plaintext[i][j] == 0)
                streamPlain[num++] = '0';
            else
                streamPlain[num++] = '1';
            cout << plaintext[i][j];
        }
        cout << endl;
    }
    cout << endl;
}

void initial(int(&plainTemp)[800][64], int(&initialPlain)[800][64], string st) {	//	DES初始置换
    int block;
    int i, j;
    block = (st.length() - 1) / 8;
    for (i = 0; i < block + 1; i++) {
        for (j = 0; j < 64; j++) {
            initialPlain[i][j] = plainTemp[i][(IP[j] - 1)];	//查表，初始化明文块
        }
    }
}

void makeKey(int *key, int(&subKey)[16][48], int way) {		//构造子密钥
    int i, j, l, r, k, flag, p, q;
    int temp0[56] = { 0 };
    int templeft[28] = { 0 };
    int tempright[28] = { 0 };
    int combine[56] = { 0 };
    cout << "-----------------------------------Product SubKey----------------------------------" << endl << endl;
    for (i = 0; i < 56; i++) {	//	PC-1
        temp0[i] = key[(PC_1[i] - 1)];
        if (i < 28) {
            templeft[i] = key[(PC_1[i] - 1)];
        }
        else {
            tempright[i - 28] = key[(PC_1[i] - 1)];
        }
        //PC-1完成，输出结果用于检验
    }

    //输出PC-1
    cout << endl << "PC-1: ";
    for (i = 0; i < 56; i++) {
        cout << temp0[i];
    }
    cout << endl;

    //输出L0和R0
    cout << "L(0): ";
    for (i = 0; i < 28; i++) {
        cout << templeft[i];
    }

    cout << endl << "R(0): ";
    for (i = 0; i < 28; i++) {
        cout << tempright[i];
    }
    cout << endl << endl;


    for (i = 0; i < 16; i++) {	//循环左移
        if (i == 0 || i == 1 || i == 8 || i == 15) {
            l = templeft[0];
            r = tempright[0];
            for (j = 0; j < 27; j++) {
                templeft[j] = templeft[j + 1];
                tempright[j] = tempright[j + 1];
            }
            templeft[j] = l;
            tempright[j] = r;	//移完一次 储存

                                //输出L和R
            cout << "L(" << i + 1 << "): ";
            for (p = 0; p < 28; p++) {
                cout << templeft[p];
            }
            cout << endl << "R(" << i + 1 << "): ";
            for (p = 0; p < 28; p++) {
                cout << tempright[p];
            }

            //组合L和R
            for (flag = 0; flag < 56; flag++) {
                if (flag < 28) {
                    combine[flag] = templeft[flag];
                }
                else {
                    combine[flag] = tempright[flag - 28];
                }
            }
            //PC-2
            cout << endl << "SubKey" << i + 1 << ": ";
            if (way == 0) {	//加密
                for (flag = 0; flag < 48; flag++) {
                    subKey[i][flag] = combine[(PC_2[flag] - 1)];
                    cout << subKey[i][flag];
                }
            }
            else {	//	解密子密钥顺序相反
                for (flag = 0; flag < 48; flag++) {
                    subKey[15 - i][flag] = combine[(PC_2[flag] - 1)];
                    cout << subKey[15 - i][flag];
                }
            }
            cout << endl << endl;
        }
        else {
            for (k = 0; k < 2; k++) {
                l = templeft[0];
                r = tempright[0];
                for (j = 0; j < 27; j++) {
                    templeft[j] = templeft[j + 1];
                    tempright[j] = tempright[j + 1];
                }
                templeft[j] = l;
                tempright[j] = r;	//移完两次 储存
            }

            //输出L和R
            cout << "L(" << i + 1 << "): ";
            for (p = 0; p < 28; p++) {
                cout << templeft[p];
            }
            cout << endl << "R(" << i + 1 << "): ";
            for (p = 0; p < 28; p++) {
                cout << tempright[p];
            }


            for (flag = 0; flag < 56; flag++) {
                if (flag < 28) {
                    combine[flag] = templeft[flag];
                }
                else {
                    combine[flag] = tempright[flag - 28];
                }
            }
            //PC-2

            cout << endl << "SubKey" << i + 1 << ": ";
            if (way == 0) {	//加密
                for (flag = 0; flag < 48; flag++) {
                    subKey[i][flag] = combine[(PC_2[flag] - 1)];
                    cout << subKey[i][flag];
                }
            }
            else {	//解密密钥顺序相反
                for (flag = 0; flag < 48; flag++) {
                    subKey[15 - i][flag] = combine[(PC_2[flag] - 1)];
                    cout << subKey[15 - i][flag];
                }
            }
            cout << endl << endl;
        }
    }
    cout << "------------------------------SubKey Information-----------------------------" << endl << endl;

    for (i = 0; i < 16; i++) {
        cout << "SubKey" << i + 1 << ": ";
        for (flag = 0; flag < 48; flag++) {
            cout << subKey[i][flag];
        }
        cout << endl;
    }
    cout << endl;

}

void funEBox(int extrightPlain[48], int tempright[32]) {		//扩展置换(EBox)
    int i;
    for (i = 0; i < 48; i++) {
        extrightPlain[i] = tempright[(EBox[i] - 1)];
        cout << extrightPlain[i];
    }
    cout << endl;
}

void funSBox(int key[][48], int extrightPlain[48], int out[32], int j) {		//代替函数组(SBox)
    int m, n, p, s, t, k, q;
    int temp[8][6] = { 0 };
    int temp2[8] = { 0 };

    cout << endl << "   SubKey(" << j + 1 << "): ";
    for (m = 0; m < 48; m++) {
        cout << key[j][m];
    }
    cout << endl << "   Key XOR RP: ";
    p = 0;
    for (m = 0; m < 8; m++) {
        for (n = 0; n < 6; n++) {
            temp[m][n] = (key[j][p]) ^ (extrightPlain[p]);
            cout << temp[m][n];
            p++;
        }
    }

    for (m = 0; m < 8; m++) {
        s = (temp[m][0]) * 2 + temp[m][5];
        t = (temp[m][1] * 8 + temp[m][2] * 4 + temp[m][3] * 2 + temp[m][4]);
        temp2[m] = SBox[m][s][t];

    }

    //二进制转换
    m = 0;
    for (k = 0; k < 8; k++) {
        t = temp2[7 - k];		//二进制转换
        for (q = 0; q < 4; q++) {

            out[32 - 1 - m] = t % 2;
            t /= 2;
            m++;
        }

    }

    cout << endl << "   OutPut: ";
    for (m = 0; m < 32; m++) {
        cout << out[m];
    }
    cout << endl;

}

void funPBox(int outSBox[32], int outPBox[32]) {		//置换运算(PBox)
    int i;
    for (i = 0; i < 32; i++) {
        outPBox[i] = outSBox[(PBox[i] - 1)];
        cout << outPBox[i];
    }
    cout << endl << endl;
}

void funFinalTP(int left[32], int right[32], int finalTrans[64], int i, int(&finalresult)[800][64]) {//逆初始置换
    int j;
    int tempfinalTrans[64] = { 0 };

    for (j = 0; j < 64; j++) {
        if (j < 32) {
            tempfinalTrans[j] = left[j];
        }
        else {
            tempfinalTrans[j] = right[j - 32];
        }
    }
    cout << endl << "Block" << i + 1 << " Ciphertext" << ": ";
    for (j = 0; j < 64; j++) {
        finalTrans[j] = tempfinalTrans[(FT[j] - 1)];
        finalresult[i][j] = finalTrans[j];
        cout << finalTrans[j];
    }
    cout << endl;
}

void outputchar(int(&finalresult)[800][64], int i, int way,char Cipher[500]) {
    ofstream fout;
    int temp[800] = { 0 };
    int t, j, q, sum, m, n;
    char ch[1000] = { '\0' };
    sum = 0;
    q = 0;
    way = 0;
    for (t = 0; t < i; t++) {
        for (j = 0; j < 64; j++) {
            m = 7 - (j % 8);
            n = pow(2, m);
            sum = sum + (finalresult[t][j])*n;
            if (m == 0) {
                temp[q] = sum;
                q++;
                sum = 0;
            }
        }
    }

    //输出ASCII对应的字符
    cout << endl << endl << "Character Ciphertext: " << endl << endl;
    for (m = 0; m < q; m++) {
     //   ch[m] = temp[m];
     //   cout << ch[m];
      Cipher[m] = temp[m];
      cout<<Cipher[m];

    }


    if (way == 0) {
        fout.open("DESciphertext.txt");	//打开DESciphertext.txt文件
        fout << ch;
        fout.close();
        cout << endl << endl << endl;
        cout << "-------------------------------" << endl;
        cout << "Saved in the DESciphertext.txt";
    }

    /*
    else {
        fout.open("DESplaintext.txt");	//打开DESciphertext.txt文件
        fout << ch;
        fout.close();
        cout << endl << endl << endl;
        cout << "------------------------------" << endl;
        cout << "Saved in the DESplaintext.txt";
    }

    cout << endl << endl;
*/
}

/*
string readFileIntoString(char * filename) {		//从文件读入到string里
    cin >> filename;
    strcat(filename, ".txt");
    fstream openencryfile(filename);
    if (!openencryfile)
    {
        cout << "Open failed!" << endl << endl;
        exit(1);
    }
    ifstream ifile(filename, _IOSbinary);	//文件以二进制形式打开
    ostringstream buf;		//将文件读入到ostringstream对象buf中
    char ch;
    while (buf && ifile.get(ch))
        buf.put(ch);
    return buf.str();		//返回与流对象buf关联的字符串
}
*/
void DES(string key, string st, char streamCipher[500], char Cipher[500], char streamPlain[500]){

    string strchoose;
    fstream opendecryfile;
    fstream openencryfile;
    char choose;
    char filename[30];
    char * fn = filename;
    int i, j, block, m, n, flag, way;
    int num = 0;
    int aa;
    char bb;
    int *temp;		//指向二进制密钥的指针
    int keytemp[64] = { 0 };
    int pltext[800][64] = { 0 };
    int leftPlain[800][32] = { 0 };
    int rightPlain[800][32] = { 0 };
    int extrightPlain[48] = { 0 };
    int iniPlain[800][64] = { 0 };
    int subKey[16][48] = { 0 };
    int outSBox[32] = { 0 };
    int outPBox[32] = { 0 };
    int templeft[32] = { 0 };
    int tempright[32] = { 0 };
    int temptrans[32] = { 0 };
    int finalTrans[64] = { 0 };
    int finalresult[800][64] = { 0 };	//将各block密文组合到一块储存起来

    way = 0;    //加密

        block = (st.length() - 1) / 8;	//	明文分组，共block+1块
        plainTrans(st, block, pltext, way, streamPlain);
        temp = keyTrans(key, keytemp);	//密钥二进制转换

        makeKey(temp, subKey, way);		//得到子密钥

        initial(pltext, iniPlain, st);	//DES初始置换


        for (i = 0; i < block + 1; i++) {		//将初始化了的明文分离存入数组，得到LP和RP

            for (j = 0; j < 64; j++) {
                if (j < 32) {
                    leftPlain[i][j] = iniPlain[i][j];
                }
                else {
                    rightPlain[i][j - 32] = iniPlain[i][j];
                }
            }
        }
        for (i = 0; i < block + 1; i++) {	//处理的分块

            for (flag = 0; flag < 32; flag++) {		//明文初始块的LP和RP

                templeft[flag] = leftPlain[i][flag];
                tempright[flag] = rightPlain[i][flag];
            }
            cout << endl << "------------------------------Block " << i + 1 << " --------------------------------" << endl;
            cout << endl << "Block" << i + 1 << ":";
            for (m = 0; m < 64; m++) {
                cout << pltext[i][m];
            }
            cout << endl;

            cout << "IT: ";
            for (m = 0; m < 64; m++) {
                cout << iniPlain[i][m];
            }

            cout << endl << endl << "LP(0): ";
            for (flag = 0; flag < 32; flag++) {
                cout << templeft[flag];
            }
            cout << endl;

            cout << "RP(0): ";
            for (flag = 0; flag < 32; flag++) {
                cout << tempright[flag];
            }
            cout << endl;

            for (j = 0; j < 16; j++) {	//每次加密需要循环16次
                cout << "EBox" << j << ": ";	//Ebox扩展置换
                funEBox(extrightPlain, tempright);
                cout << "SBox" << j << ": ";
                funSBox(subKey, extrightPlain, outSBox, j);
                cout << "PBox" << j << ": ";
                funPBox(outSBox, outPBox);
                for (m = 0; m < 32; m++) {
                    temptrans[m] = tempright[m];
                    tempright[m] = (templeft[m]) ^ (outPBox[m]);
                    templeft[m] = temptrans[m];
                }

                if (j == 15) {
                    for (m = 0; m < 32; m++) {
                        temptrans[m] = tempright[m];
                        tempright[m] = templeft[m];
                        templeft[m] = temptrans[m];
                    }
                }
                cout << endl << "LP(" << j + 1 << "): ";
                for (flag = 0; flag < 32; flag++) {
                    cout << templeft[flag];
                }

                cout << endl << "RP(" << j + 1 << "): ";
                for (flag = 0; flag < 32; flag++) {
                    cout << tempright[flag];
                }
                cout << endl;
            }
            funFinalTP(templeft, tempright, finalTrans, i, finalresult);
        }
        cout << endl << "------------------------------Final Result --------------------------------" << endl;
        cout << endl << "Stream Ciphertext: " << endl << endl;
        for (i = 0; i < block + 1; i++) {
            for (j = 0; j < 64; j++) {
                cout << finalresult[i][j];
                if(finalresult[i][j] == 0)
                    streamCipher[num++] = '0';
                else
                   streamCipher[num++] = '1';
                //streamCipher[num++] = (char)aa;
            }
            cout << endl;
        }

        //由ASCII码输出字符
       // cout<<"test   test   test"<<endl;
      //  for(i = 0; i< num; i++)
      //     cout<<streamCipher[i];





        //输出ASCII对应的字符
    //    cout << endl << endl << "Character Ciphertext: " << endl << endl;
    //    for (m = 0; m < q; m++) {
    //        ch[m] = temp[m];
   //         cout << ch[m];
          //Cipher[m] = tempp[m];
         // cout<<tempp[m];

   //     }




        outputchar(finalresult, i, way, Cipher);
        //return streamCipher;
    }

