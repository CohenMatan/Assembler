#include "tables.h"

void structReset();
char *strdup(const char *);
int checkLabel(const char *tempWord, int bufIndex);
int * spaceJump(const char *buf, int *index);
char * getWord(const char *buf,int *bufIndex,int *errorNum);
char * stringArgu(const char *buf, int *bufIndex,int * errorNum);
int * dataArgu(const char *buf, int *bufIndex,int *errorNum);
char * extEntArgu(const char *buf, int *bufIndex,int *errorNum);
void labelCmp(char *labelWord, int *errorNum, symbol *ptr, int numOfSymbols,int externFlag);
void checkMode(int arrIndex, int addressing, int *errorNum, int num);
char * getArgu(const char *buf, int *bufIndex, char * argu,int *errorNum);
int numsOfArgu(int arrIndex);
int addressingMode(const char *argu,int * errorNum);
int howMany(int *arguArr);
 void swap(char *x, char *y);
char* itoa(int value, char* buffer, int base);
char* instructionWords(char *buffer);
