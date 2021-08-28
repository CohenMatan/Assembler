#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "errors.h"
#include "tables.h"
#define CharsInLabel 31
#define CharsInLine 81
#define MaxPosInt 2047
#define MaxNegInt -2048


/*Jump spaces and tabs between words*/
int *spaceJump(const char *buf, int *index){
    int c;
    while((c=(int)buf[*index]) == ' ' || (c=(int)buf[*index]) == '\t' || (c=(int)buf[*index]) == '\r')
        *index += 1;
    return index;
}


/*Gets the string and return one word. */
char *getWord(const char *buf,int *bufIndex,int *errorNum) {
    int c;
    int tempIndex = 0;
    char tempArr[CharsInLine];
    char *tempWord = "";

    if ((c=(int) buf[*bufIndex]) != '\0' && (c=(int) buf[*bufIndex]) !='\n' && (c=(int)buf[*bufIndex]) != '\r' ) {
        bufIndex = spaceJump(buf,bufIndex);
        while (((c=(int) buf[*bufIndex]) != ' ') && ((c=(int) buf[*bufIndex]) != '\t') && ((c=(int) buf[*bufIndex]) != '\0') && ((c=(int) buf[*bufIndex]) != '\n' ) && (c=(int) buf[*bufIndex]) != '\r' && (c=(int) buf[*bufIndex]) != ','){
            tempArr[tempIndex] = (char)c;
            tempIndex++;
            *bufIndex += 1;
        }
        if(tempIndex>CharsInLabel)
            *errorNum = -12;
        else{
            tempArr[tempIndex] = '\0';
            tempWord = strdup(tempArr);
        }
    }
    return tempWord;
}


/*Checks if the word its a legal label.*/
int checkLabel( char *tempWord, int bufIndex){
    int i;
    int result = 0;
    char *reservedWords [24] = {"mov:","cmp:","add:","sub:","lea:","clr:","not:","inc:","dec:","jmp:","bne:","jsr:","red:",
                               "prn:","rts:","stop:","r0:","r1:","r2:","r3:","r4:","r5:","r6:","r7:"}; /*24=all the reserved words*/
    for(i=0; i <= 23 ;i++){
        if (!strcmp(tempWord,reservedWords[i])){
            return -3;
        }
    }
    for(i=0; i<bufIndex-1 ;i++){/* Checks if there is more then one of: ':' */
        if(tempWord[i] == ':')
            result++;
    }
    if(result >= 2){
        return -4;
    }
    if ((isupper(tempWord[0]) || islower(tempWord[0])) && strlen(tempWord) <= CharsInLabel) {
        if(tempWord[strlen(tempWord)-1] == ':')
            tempWord[strlen(tempWord)-1] = '\0';
        return 1;
    }
    return -4;
}


/*Checks if .string argument is legal. If its not the function change the value of "errorFlag" to -1*/
char * stringArgu(const char *buf, int *bufIndex,int *errorNum){
    int c;
    int arguIndex = 0;
    char tempArr[CharsInLine];
    char *arguArr = "";

    bufIndex = spaceJump(buf, bufIndex);
    if ((c=(int) buf[*bufIndex]) == '\"'){ /*If the first char of the argument is '\"'*/
        *bufIndex += 1;
    }
    else
        *errorNum = -5;

    while((c=(int) buf[*bufIndex]) != '\0' && (c=(int) buf[*bufIndex]) != '\n' && (c=(int) buf[*bufIndex]) != '\r' && *errorNum==0 ){
        while ((c=(int) buf[*bufIndex]) != '\"' && (c=(int) buf[*bufIndex]) != '\0' && (c=(int) buf[*bufIndex]) != '\n' ){
            if((c=(int) buf[*bufIndex]) == '\n'){
                *errorNum = -5;
                break;
            }
            tempArr[arguIndex] = buf[*bufIndex];
            arguIndex++;
            *bufIndex += 1;
        }
        if ((c=(int) buf[*bufIndex]) == '\"')
            tempArr[arguIndex] = '\0';
        else{
            *errorNum = -5;
            break;
        }
        *bufIndex += 1;
        arguArr = strdup(tempArr);
        bufIndex = spaceJump(buf,bufIndex);
        if(buf[*bufIndex] != '\0' && buf[*bufIndex] != '\n' && buf[*bufIndex] != '\r')
            *errorNum = -5;
    }
    if((buf[*bufIndex] == '\0' || buf[*bufIndex] == '\n') && arguIndex == 0) /*No argument entered*/
        *errorNum = -5;
    return arguArr;
}


/*Checks if .data argument is legal. If its not the function change the value of "errorFlag" to -1*/
int *dataArgu(const char *buf, int *bufIndex,int *errorNum){
    int c;
    int val;
    int tempIndex = 0;
    int arguIndex = 0;
    int static arrToCopy[CharsInLine];
    int *arguArr = NULL;
    char tempArr[CharsInLine];

    bufIndex = spaceJump(buf, bufIndex);
    if(buf[*bufIndex]==',')  /*If the first argument start with ','*/
        *errorNum = -5;

    if (buf[*bufIndex]=='\n' || buf[*bufIndex]=='\0') /*No argument entered*/
        *errorNum = -6;

    while ((c=(int) buf[*bufIndex]) != '\0' && (c=(int) buf[*bufIndex]) != '\r' && (c=(int) buf[*bufIndex]) != '\n' && *errorNum==0){
        while ((((c=(int) buf[*bufIndex]) == '+') || ((c=(int) buf[*bufIndex]) == '-') || isdigit((c=(int) buf[*bufIndex]))) && (c=(int) buf[*bufIndex]) != '\0' ){
            if(tempIndex != 0 && (((c=(int) buf[*bufIndex]) == '+') || ((c=(int) buf[*bufIndex]) == '-'))){/*Checks if there is more then one + or -, and if its not the first char*/
                *errorNum = -5;
                break;
            }
            tempArr[tempIndex]=(char)c;
            tempIndex++;
            *bufIndex += 1;
        }
        if(*errorNum == -5)
            break;

        tempArr[tempIndex] = '\0';
        tempIndex = 0;
        val = atoi(tempArr);
        if(val > MaxPosInt || val < MaxNegInt){
            *errorNum = -17;
            break;
        }
        if(atoi(tempArr) || tempArr[tempIndex] == '0')
            arrToCopy[arguIndex] = val;
        else{
            *errorNum = -5;
            break;
        }
        bufIndex = spaceJump(buf, bufIndex);
        if ((c=(int) buf[*bufIndex]) != ',' && (c=(int) buf[*bufIndex]) != '\0' && (c=(int) buf[*bufIndex]) != '\n' && (c=(int) buf[*bufIndex]) != '\r') {
            *errorNum = -5;
            break;
        }
        if ((c=(int) buf[*bufIndex]) == ','){
            *bufIndex += 1;
            bufIndex = spaceJump(buf, bufIndex);
            if(!isdigit(c=(int) buf[*bufIndex])  && ((c=(int) buf[*bufIndex]) != '-' && (c=(int) buf[*bufIndex]) != '+'))
                *errorNum = -5;
        }
        arguIndex++;
        tempIndex = 0;
        bufIndex = spaceJump(buf, bufIndex);
    }
    arrToCopy[arguIndex]='$'; /* $ is to mark there is no more arguments */
    if (*errorNum == 0)
        arguArr = arrToCopy;
    return arguArr;
}


/*Checks if .extern argument is legal.*/
char * extEntArgu(const char *buf, int *bufIndex,int *errorNum){
    int c;
    int tempIndex = 0;
    int labelFlag = 0;
    char *arguArr = "";
    char tempArr[CharsInLine];

    while(tempIndex < CharsInLine){
        tempArr[tempIndex] = '\0';
        tempIndex++;
    }
    tempIndex = 0;

    bufIndex = spaceJump(buf, bufIndex);
    if((c=(int) buf[*bufIndex]) == '\0' || (c=(int) buf[*bufIndex]) == '\n'){
        *errorNum = -6;
        return arguArr;
    }
    while((c=(int) buf[*bufIndex]) != '\0' && (c=(int) buf[*bufIndex]) != '\n' && (c=(int) buf[*bufIndex]) != ' ' && (c=(int) buf[*bufIndex]) != '\t' && (c=(int) buf[*bufIndex]) != '\r'){
        tempArr[tempIndex] = buf[*bufIndex];
        *bufIndex += 1;
        tempIndex++;
    }
    bufIndex = spaceJump(buf, bufIndex);
    if ((c=(int) buf[*bufIndex]) == '\0' || (c=(int) buf[*bufIndex]) == '\n') /*Checks if there is another chars after the accepted word */
        labelFlag = checkLabel(tempArr,*bufIndex);
    else{
        *errorNum = -8;
        return arguArr;
    }
    if(labelFlag == 1)
        arguArr = strdup(tempArr);
    return arguArr;
}


/*Checks how many arguments expected to the current sentence and return it */
int numsOfArgu(int arrIndex){
    if(arrIndex >= 0 && arrIndex <= 4)
        return 2;
    else if (arrIndex >= 5 && arrIndex <= 13)
        return 1;
    else
        return 0;
}


/*For instruction word. Return argument */
char * getArgu(const char *buf, int *bufIndex,char * argu,int* errorNum){
    int i = 0;
    int c;
    char arr[CharsInLine];

    while(i < CharsInLine){
        arr[i] = '\0';
        i++;
    }
    i = 0;

    bufIndex = spaceJump(buf, bufIndex);
    if(buf[*bufIndex] == ',') /*If the first argument start with ','*/
        *errorNum = -5;

    if (buf[*bufIndex] == '\n' || buf[*bufIndex] == '\0') /*No argument entered*/
        *errorNum = -6;
    else {
        while ((c=(int) buf[*bufIndex]) != '\0' && (c=(int) buf[*bufIndex]) != '\n' && (c=(int) buf[*bufIndex]) != '\t' && (c=(int) buf[*bufIndex]) != ' ' && (c=(int) buf[*bufIndex]) != ',' && (c=(int) buf[*bufIndex]) != '\r'){
            arr[i] = buf[*bufIndex];
            i++;
            *bufIndex += 1;
        }
        bufIndex = spaceJump(buf, bufIndex);
        arr[i] = '\0';
        argu = strdup(arr);
    }
    return  argu;
}


/*Checks what addressing mode the current argument*/
int addressingMode( char *argu,int * errorNum){
    if(argu[0] == '#')
        return 0;
    else if (argu[0] == '%')
        return 2;
    else if(argu[0] == 'r' && argu[1] >= '0' && argu[1] <= '7')
        return 3;
    else if (checkLabel(argu,(int)strlen(argu)) == 1)
        return 1;
    *errorNum = -5;
    return -1;
}


/*check if the arguments of the action word match to the addressing mode*/
void checkMode(int arrIndex, int addressing, int *errorNum, int num){
    enum arrIndex{mov,cmp,add,sub,lea,clr,not,inc,dec,jmp,bne,jsr,red,prn,rts,stop};
    switch (arrIndex){
        case mov:
        case sub:
        case add:
        {
            if (num == 1) {
                if (addressing == 0 || addressing == 1 || addressing == 3)
                    break;
                else
                    *errorNum = -9;
            }
            else {/*num=2*/
                if(addressing == 1 || addressing == 3)
                    break;
                else
                    *errorNum = -9;
            }
        }
        case cmp:{
            if(addressing == 0 || addressing == 1 || addressing == 3)
                break;
            else *errorNum = -9;
        }
        case lea:{
            if (num == 1) {
                if (addressing == 1 )
                    break;
                else
                    *errorNum = -9;
            }
            else {/*num=2*/
                if(addressing == 1 || addressing == 3)
                    break;
                else {
                    *errorNum = -9;
                }
            }
        }
        case clr:
        case not:
        case inc:
        case dec:
        case red:
        {
            if(addressing == 1 || addressing == 3)
                break;
            else *errorNum = -9;
        }
        case jmp:
        case bne:
        case jsr:
        {
            if(addressing == 1 || addressing == 2)
                break;
            else *errorNum = -9;
        }
        case prn:
        {
            if(addressing == 0 || addressing == 1 || addressing == 3)
                break;
            else *errorNum = -9;
        }
    }
}


/*Checks if the CheckLabel is already in the symbol table. */
void labelCmp(char *labelWord, int *errorNum, symbol *ptr, int numOfSymbols,int externFlag){
    int i = 0;
    while (i != numOfSymbols-1) {
        if (!strcmp(labelWord, ptr[i].label)) {
            if(!strcmp(ptr[i].attributes,"external") && externFlag == 1)
                break;
            *errorNum = -11;
            break;
        }
        i++;
    }
}


/*Checks how many members in the array of the arguments*/
int howMany(const int *arguArr,int arrIndex){
    int i = 0;
    int num = 0;
    while (arguArr[i] != '$'){
        num++;
        i++;
    }
    return num;
}


/*Function to swap two numbers*/
void swap(char *x, char *y) {
    char t = *x;
    *x = *y;
    *y = t;
}


/*Function to reverse buffer[i..j]*/
char* reverse(char *buffer, int i, int j){
    while (i < j)
        swap(&buffer[i++], &buffer[j--]);

    return buffer;
}


char* itoa(int value, char* buffer, int base){
    int n;
    int i = 0;
    int r;

    /*invalid input*/
    if (base < 2 || base > 32)
        return buffer;

    n = abs(value);
    while (n){
        r = n%base;
        if (r >= 10)
            buffer[i++] = 65+(r-10);
        else
            buffer[i++] = 48+r;
        n = n/base;
    }
    /*If the number is 0*/
    if (i == 0)
        buffer[i++] = '0';

    if (value < 0 && base == 10)
        buffer[i++] = '-';

    buffer[i] = '\0';

    return reverse(buffer,0,i-1);
}


/*Checks if the word is equal to one of the instruction words*/
int instructionWords(char *word){
    char *arr[]={"mov","cmp","add","sub","lea","clr","not","inc","dec","jmp","bne","jsr","red","prn"};
    int i = 0;
    while(i < 14){
        if(!strcmp(word, arr[i]))
            return 1;
        i++;
    }
    return 0;
}


/*reset the all the structures*/
void structReset(){
    extern int IC;
    extern int DC;
    int i;
    if (ptrSymbol != NULL){
        for(i=0; i < ptrSymbol[0].numOfSymbols-1 ;i++){
            free(ptrSymbol[i].label);
	    if(strlen(ptrSymbol[i].attributes) == 11 )            
		free(ptrSymbol[i].attributes);
        }
    }
    for(i=0;i < IC+DC-100; i++){
        if (ptrCode[i].feature == 'E')
            free(ptrCode[i].externWord);

        if (strcmp(ptrCode[i].binStr,"?"))
            free(ptrCode[i].binStr);
    }
    free(ptrData);
    free(ptrSymbol);
    free(ptrCode);
    ptrData = NULL;
    ptrCode = NULL;
    ptrSymbol = NULL;
}
