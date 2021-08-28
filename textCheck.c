#include <stdlib.h>
#include <string.h>
#include "errors.h"
#include "utils.h"
#define CharsInLine 81

/*The function checks what is the guidance word and continue to another function to check if the arguments of the guidance word is legal.
 If it is legal the program continue to encoding*/
void guidanceArgu(const char *buf, int * bufIndex, int arrIndex, char *label, int *errorNum){
    extern int DC;
    extern int IC;
    char *arguArr = "";
    int *arguForData = NULL;

    if(arrIndex == 0)/* For: .string */
        arguArr = stringArgu(buf,bufIndex,errorNum);

    else if(arrIndex == 1) /* For: .data  */
        arguForData = dataArgu(buf, bufIndex, errorNum);

    else {/*For: .extern or .entry*/
        if(*label != '\0')
            *errorNum = -10;
        arguArr = extEntArgu(buf,bufIndex,errorNum);
        if(arguArr == '\0' ){ /*Argument is missing.*/
            *errorNum = -6;
        }
    }

    if (arrIndex == 2 && *errorNum != -6 ){
        if(ptrSymbol != NULL){
            labelCmp(arguArr,errorNum,ptrSymbol,ptrSymbol[0].numOfSymbols,1);
            if(*errorNum != -11)
                symbolTable(arguArr,0,"external",errorNum);
        }
        else symbolTable(arguArr,0,"external",errorNum);
    }
    else if( *label != '\0' && *errorNum == 0  ){/*Add the symbol to the symbol table*/
        symbolTable(label,DC+IC,"data",errorNum);
    }

    if (*errorNum == 0 && (arrIndex == 0 || arrIndex == 1)){/*Continue to encoding*/
        if (arrIndex == 1)
            dataTable(arguForData,arrIndex,errorNum);
        else
            dataTable((int *) arguArr, arrIndex, errorNum);
    }
    if (*arguArr != '\0')
	free(arguArr);
}

/*Checks if the argument of the instruction and is type is legal.If it is legal the program continue to encoding*/
void instructionArgu(const char *buf, int *bufIndex, int arrIndex,char *label,int *errorNum){
    extern int IC;
    int i = 0;
    int NumOfArgu = 0;
    int first = 1;
    int sec = 2;
    int firstAddressing = -1;
    int secAddressing = -1;
    char *firstArgu = "";
    char *secArgu = "";
    bufIndex = spaceJump(buf, bufIndex);
    NumOfArgu = numsOfArgu(arrIndex);
    
    while(i < NumOfArgu && *errorNum == 0){
        if (i == 0) {
            firstArgu=getArgu(buf,bufIndex,firstArgu,errorNum);
        }
        else {
            *bufIndex += 1;
            secArgu = getArgu(buf, bufIndex, secArgu, errorNum);
        }
        i++;
    }
    if((buf[*bufIndex] == '\0' || buf[*bufIndex] == '\n' || buf[*bufIndex] == '\r') && i <= NumOfArgu){
        while( i != 0 && *errorNum == 0){
            if(i == 1){
                firstAddressing = addressingMode(firstArgu,errorNum);
            }
            else{/*i==2*/
                secAddressing = addressingMode(secArgu,errorNum);
            }
            i--;
        }
        /*Send to function the check if the arguments of the action word match to the addressing mode*/
        if (firstAddressing != -1){
            checkMode(arrIndex,firstAddressing,errorNum,first);
            if(firstAddressing == 0 && *errorNum == 0){/*To check if the argument its a number.*/
                *bufIndex = 1;
                dataArgu(firstArgu,bufIndex,errorNum);
            }
            else if ((firstAddressing == 2 || firstAddressing == 1) && *errorNum == 0 ) {/*To check if the argument its a legal label.*/
                if (firstAddressing == 2){
                    i = 0;
                    while(i < strlen(firstArgu)-1){
                        firstArgu[i]= firstArgu[i+1];
                        i++;
                    }
                    firstArgu[i] = '\0';
                }
                *errorNum = checkLabel(firstArgu, 0);
                if (*errorNum == 1)
                    *errorNum = 0;
            }
        }
        if(secAddressing != -1){
            checkMode(arrIndex,secAddressing,errorNum,sec);
            if(secAddressing == 0 && *errorNum == 0){/*To check if the argu its a number.*/
                *bufIndex = 1;
                dataArgu(secArgu,bufIndex,errorNum);
            }
            else if ((secAddressing == 2 || secAddressing == 1) && *errorNum == 0 ) {/*To check if the argument its a legal label.*/
                if (secAddressing == 2){
                    i = 0;
                    while(i < strlen(secArgu)-1){
                        secArgu[i] = secArgu[i+1];
                        i++;
                    }
                    secArgu[i]='\0';
                }
                *errorNum = checkLabel(secArgu,0);
                if (*errorNum == 1)
                    *errorNum = 0;
            }
        }
        if(*label != '\0' && *errorNum == 0)/*Add the symbol to the symbol table*/
            symbolTable(label,IC,"code",errorNum);

        if(*errorNum == 0)/* Send to encoding.*/
            codTable(arrIndex, firstAddressing, secAddressing,firstArgu,secArgu, NumOfArgu,errorNum);
    }
    else if(*errorNum == 0){
        *errorNum = -8;
    }
    if(*firstArgu != '\0')
        free(firstArgu);
    if (*secArgu != '\0')
        free(secArgu);
}

/*Checks if the word its a guidance sentence or instructional sentence*/
void whichSentence(const char *buf, int *bufIndex, const char *tempWord, int labelFlag, int * errorNum) {
    int arrIndex;
    char *sentence[] = {".string",".data",".extern",".entry","mov","cmp","add","sub","lea","clr","not","inc","dec","jmp","bne","jsr","red","prn","rts","stop"};
    char *nextWord = "";
    char *label = "";

    if (labelFlag == 1){/*Checks if flag CheckLabel is on.If its on, we get the next word from the line. If its not we use the current word*/
        nextWord = getWord(buf,bufIndex,errorNum);
        label = (char*)tempWord;
        tempWord = nextWord;
    }
    if (tempWord[0] == '.'){/*Checks if the word its a guidance sentence*/
        for(arrIndex=0;arrIndex<4;arrIndex++) {
            if (!strcmp(tempWord, sentence[arrIndex])) {
                guidanceArgu(buf, bufIndex,arrIndex,label,errorNum);
                break;
            }
        }
        if(arrIndex == 4)
            *errorNum = -7;
    }
    else {/*Checks if the word its a instructions sentence*/
        for(arrIndex=4;arrIndex<20;arrIndex++){
            if (!strcmp(tempWord,sentence[arrIndex])){
                arrIndex -= 4;/*The action words start in the array in index 4*/
                instructionArgu(buf,bufIndex,arrIndex,label,errorNum);
                break;
            }
        }
        if (arrIndex == 20){/*If the word not found in the array*/
            *errorNum = -7;
        }
    }
    if (*nextWord != '\0')
        free(nextWord);	
}

/* Reads the first word from the line to check if its a CheckLabel */
void firstWord(const char *buf,int *errorNum){
    int *bufIndex;
    int index4Buf = 0;
    int labelFlag = 0;
    char *tempWord = "";
    bufIndex = &index4Buf;
    tempWord = getWord(buf,bufIndex,errorNum);

    if (strlen(buf) >= CharsInLine)
        *errorNum = -19;
    else {
        if (*errorNum == 0) {
            if (tempWord[0] == ';' || tempWord[0] == '\0') {/*Comment line or empty line*/
                ;
            } else if (buf[*bufIndex - 1] == ':') {/*Checks if the word is a legal CheckLabel word.If it is so the word saves in labelWord. if its not, return to get new line*/
                labelFlag = checkLabel(tempWord, *bufIndex);
                if (labelFlag < 0) {
                    *errorNum = labelFlag;
                } else /*continue to check if the current word or next word its guidance sentence or instructional sentence */
                    whichSentence(buf, bufIndex, tempWord, labelFlag, errorNum);
            } else
                whichSentence(buf, bufIndex, tempWord, labelFlag, errorNum);
        }
    }
    if (*tempWord != '\0')
        free(tempWord);	
}
