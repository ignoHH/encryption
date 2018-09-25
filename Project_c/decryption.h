#ifndef DECRYPTION_H
#define DECRYPTION_H
#include <string>
#include <string.h>
#include "common.h"
using namespace std;
char* decryCaesar(int, string, char*);
char* decryKeyword(string, string, char*);
char* decryVigenere(string, string, char*);
char* decryAutokey(string, string, char*);
char* decryAutokey2(string, string, char*);
char* decryPlayfair(string, string, char*);
char* decryPermutation(string, string, char*);
char* decryColumn(string, string, char*);

#endif // DECRYPTION_H
