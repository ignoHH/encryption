#ifndef ENCRYPTION_H
#define ENCRYPTION_H
#include <string.h>
#include <string>
#include "common.h"
using namespace std;

char* encryCaesar(int, string, char*);
char* encryKeyword(string, string, char*);
char* encryVigenere(string, string, char*);
char* encryAutokey(string, string, char*);
char* encryAutokey2(string, string, char*);
char* encryPlayfair(string, string, char*);
char* encryPermutation(string, string, char*);
char* encryColumn(string, string, char*);

#endif // ENCRYPTION_H
