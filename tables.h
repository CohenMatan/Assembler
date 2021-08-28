typedef struct sSymbol {
    char *label;
    int value;
    char *attributes;
    int numOfSymbols;
}symbol;
symbol *ptrSymbol;

typedef struct sCode{
    int line;
    char* binStr;
    char feature;
    char* externWord;
}codeTable;
codeTable *ptrCode;

typedef struct sData{
    int line;
    char* binStr;
    char feature;
}DataTable;
DataTable *ptrData;

char *strdup(const char *);
char actionTable(int arrIndex,int num,int i);
void symbolTable(char *tempWord,int num,char *attributes,int *errorNum);
void codTable(int arrIndex,int firstAddressing,int secAddressing,char *firstArgu,char *secArgu,int numOfArgu,int *errorNum);
void dataTable(int *arguArr,int arrIndex, int *errorNum);
void symbolUpdate( char *buf, int *bufIndex,int *errorNum,int symbolStorageIndex);
void codeUpdate( char *buf, int *bufIndex,int *errorNum);