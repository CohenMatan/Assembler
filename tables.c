#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "bin.h"
#include "textCheck.h"
#include "errors.h"
#define numOfAction 16
#define CharsInLabel 31


/*Insert the label to the symbol table. */
void symbolTable(char *labelWord,int num,char *attributes, int *errorNum) {
    extern int DC;
    int numOfSymbols;

    if (ptrSymbol == NULL) {
        numOfSymbols = 1;
        ptrSymbol = (symbol *) malloc(numOfSymbols * sizeof(symbol));
        ptrSymbol[0].numOfSymbols = numOfSymbols;
    }
    else if(*errorNum ==0 || *errorNum== -10){
        ptrSymbol = (symbol *) realloc(ptrSymbol, ptrSymbol[0].numOfSymbols * sizeof(symbol));
    }
    if(strcmp(attributes,"external"))
       labelCmp(labelWord, errorNum, ptrSymbol, ptrSymbol[0].numOfSymbols,0);
    
    if (!ptrSymbol) {
        printf("Memory allocation failed.\n");
        exit(0);
    }
    
    if (*errorNum == 0 || *errorNum == -10) {
        ptrSymbol[ptrSymbol[0].numOfSymbols - 1].label = strdup(labelWord);
        if (!strcmp(attributes,"data"))
            ptrSymbol[ptrSymbol[0].numOfSymbols - 1].value = DC;
        else
            ptrSymbol[ptrSymbol[0].numOfSymbols - 1].value = num;
        ptrSymbol[ptrSymbol[0].numOfSymbols - 1].attributes = attributes;
        ptrSymbol[0].numOfSymbols++;
        }
}


/*For quick access to the to the parameters of the action table. */
char  actionTable(int arrIndex,int num,int i){
    typedef struct {
      char *opcode;
      char *funct;
      char *name;
    }action;
    action *p;
    char temp;
    p=(action*)malloc(numOfAction*(sizeof(action))); /*16=number of actions*/
    if (p == NULL){
        printf("ERROR: MEMORY ALLOCATION FAILED.\n");
        exit(0);
    }
    p[0].opcode = "0000"; p[0].funct = "0000"; p[0].name = "mov";
    p[1].opcode = "0001"; p[1].funct = "0000"; p[1].name = "cmp";
    p[2].opcode = "0010"; p[2].funct = "1010"; p[2].name = "add";
    p[3].opcode = "0010"; p[3].funct = "1011"; p[3].name = "sub";
    p[4].opcode = "0100"; p[4].funct = "0000"; p[4].name = "lea";
    p[5].opcode = "0101"; p[5].funct = "1010"; p[5].name = "clr";
    p[6].opcode = "0101"; p[6].funct = "1011"; p[6].name = "not";
    p[7].opcode = "0101"; p[7].funct = "1100"; p[7].name = "inc";
    p[8].opcode = "0101"; p[8].funct = "1101"; p[8].name = "dec";
    p[9].opcode = "1001"; p[9].funct = "1010"; p[9].name = "jmp";
    p[10].opcode = "1001"; p[10].funct = "1011"; p[10].name = "bne";
    p[11].opcode = "1001"; p[11].funct = "1100"; p[11].name = "jsr";
    p[12].opcode = "1100"; p[12].funct = "0000"; p[12].name = "red";
    p[13].opcode = "1101"; p[13].funct = "0000"; p[13].name = "prn";
    p[14].opcode = "1110"; p[14].funct = "0000"; p[14].name = "rts";
    p[15].opcode = "1111"; p[15].funct = "0000"; p[15].name = "stop";

    if(num==0)
        temp=p[arrIndex].opcode[i];
    else
        temp=p[arrIndex].funct[i-4];
    
    free(p);
    return temp;
}


/*Creating the code table and entering values in it. (passage 1)*/
void codTable(int arrIndex,int firstAddressing,int secAddressing,char *firstArgu,char *secArgu,int numOfArgu,int *errorNum){
    extern int IC;
    int static numOfLines=0;
    char *binary = "";

    numOfLines += numOfArgu+1;
    binary = sourceToBin(arrIndex, firstAddressing, secAddressing);

    if (ptrCode == NULL){
        ptrCode = (codeTable*) malloc(numOfLines * sizeof (codeTable));
    }
    else{
        ptrCode = (codeTable*) realloc(ptrCode,(numOfLines * sizeof (codeTable)));
    }
    if(!ptrCode){
        *errorNum = -13;
    }
    else{
        ptrCode[IC-100].line = IC;
        ptrCode[IC-100].binStr = binary;
        ptrCode[IC-100].feature = 'A';
        IC++;

        if(numOfArgu == 2){
            argusToBin(firstAddressing,firstArgu);
            IC++;
            argusToBin(secAddressing,secArgu);
            IC++;
        }
        else if (numOfArgu == 1){
            argusToBin(firstAddressing,firstArgu);
            IC++;
        }
    }
}


/*Creating the data table and entering values in it. (passage 1)*/
void dataTable(int *arguArr,int arrIndex,int *errorNum) {
    extern int DC;
    int arguNum;
    int i = 0;
    int static numOfLines=0;
    char *strArr = "";

    if (arrIndex == 0) {
        strArr = strdup((const char *)arguArr);
        arguNum = strlen((const char *)arguArr) + 1;
        strArr[arguNum-1] = 0;
    }
    else
        arguNum = howMany(arguArr);

    numOfLines += arguNum;
    if (ptrData == NULL){
        ptrData = (DataTable*) malloc(numOfLines * sizeof (DataTable));
    }
    else{
        ptrData = (DataTable*) realloc(ptrData,(numOfLines * sizeof (DataTable)));
    }
    if(!ptrData){
        *errorNum = -13;
    }
    else {
        while(arguNum != 0){
            ptrData[DC].line = DC;
            if(arrIndex == 0){
                ptrData[DC].binStr = numToBin((int)strArr[i]);
            }
            else
                ptrData[DC].binStr = numToBin(arguArr[i]);
            ptrData[DC].feature='A';
            DC++;
            i++;
            arguNum--;
        }
    }
    if(*strArr != '\0')
	free(strArr);
}


/* Update the symbol table (passage 2)*/
void symbolUpdate( char *buf, int *bufIndex,int *errorNum,int symbolStorageIndex){
    char *tempWord = "";
    char *strToCpy = "";
    int length;
    
    tempWord = getWord(buf,bufIndex,errorNum);
    if(tempWord[*bufIndex-1] == ':'){/*If the first word its a label*/
	free(tempWord);
	tempWord = getWord(buf,bufIndex,errorNum);
    }
        
   
    length = (int)strlen(tempWord);
    if (tempWord[0] == '.' && tempWord[length-1] == 'y'){
	free(tempWord);     
	tempWord = getWord(buf,bufIndex,errorNum);
        
        while(symbolStorageIndex >= 0){
            if(!strcmp(tempWord,ptrSymbol[symbolStorageIndex].label)){
                if(!strcmp(ptrSymbol[symbolStorageIndex].attributes,"external")){
                    *errorNum = -11;
                    break;
                }
                if(strlen(ptrSymbol[symbolStorageIndex].attributes) <= 4){
                    strToCpy = strdup(ptrSymbol[symbolStorageIndex].attributes);
                    strToCpy = (char*)realloc(strToCpy,8+strlen(strToCpy));
                    strToCpy = strcat(strToCpy,", entry");
                    ptrSymbol[symbolStorageIndex].attributes = strToCpy;
                }
                break;
            }
            symbolStorageIndex--;
        }
        if (symbolStorageIndex == -1 && strcmp(tempWord,"\0"))/*If the label is not found symbol in the symbol table. */
            *errorNum = -15;
    } 
    if(*tempWord != '\0')
	free(tempWord);
}


/* Update the code table (passage 2) */
void codeUpdate(char *buf, int *bufIndex,int *errorNum){
    extern int IC;
    extern int DC;
    int addressMode = -1;
    int strLen;
    int i=0;
    int symbolIndex;
    int codeStorageSize;
    char *tempWord = "";
    char *bin = "";
    
    codeStorageSize = IC+DC-100;

    spaceJump(buf,bufIndex);
    while(buf[*bufIndex] != '\0' && buf[*bufIndex] != '\n' && buf[*bufIndex] != '\r'){
        spaceJump(buf,bufIndex);
        if(addressMode != -1){
            if (buf[*bufIndex] == ',')
                *bufIndex += 1;
            else
                break;
        }
        tempWord=getWord(buf,bufIndex,errorNum);
        strLen = (int)strlen(tempWord);
        if(tempWord[strLen-1] == ':') {/* If the first word its a label */
	    free(tempWord);
            tempWord = getWord(buf, bufIndex, errorNum);
            if(!instructionWords(tempWord)){
		free(tempWord);
		break;
	    }
            free(tempWord);    
            tempWord = getWord(buf, bufIndex, errorNum);
        }
        else if( instructionWords(tempWord)) { /* Checks if its instruction word */
	    free(tempWord);
            tempWord = getWord(buf, bufIndex, errorNum);
        }
        else if(addressMode == -1){
	    free(tempWord);
 	    break;
	}
            
        addressMode = addressingMode(tempWord,errorNum);
        if (addressMode == 1 || addressMode == 2){
            if(addressMode == 2){		
                while(i < strlen(tempWord)){
                    tempWord[i] = tempWord[i+1];
                    i++;
                }
            }

            *errorNum = checkLabel(tempWord,0); /*If error returned its found at passage 1*/
            if (*errorNum == -4){
                *errorNum = 0;
		free(tempWord);
                break;
            }
            else
                *errorNum = 0;

            for(i=0;i<ptrSymbol[0].numOfSymbols-1;i++){
                if (!strcmp(ptrSymbol[i].label,tempWord)){
                    if(addressMode == 1)						
			bin = numToBin(ptrSymbol[i].value);                      
		    
                    symbolIndex = i;
                    break;
                }
            }
            if(i == ptrSymbol[0].numOfSymbols-1){
                *errorNum = -15;
		free(tempWord);
                break;
            }
            for(i=0;i < codeStorageSize;i++){
                if (!strcmp(ptrCode[i].binStr,"?")){
                    if (addressMode == 2)
 			bin = numToBin((ptrSymbol[symbolIndex].value) - ((ptrCode[i].line)));
		    
                    ptrCode[i].binStr = bin;
                    if(!strcmp(ptrSymbol[symbolIndex].attributes,"external")){
                        ptrCode[i].feature = 'E';
                        ptrCode[i].externWord = tempWord;
                    }
                    else if(ptrCode[i].feature == '?'){
			free(tempWord);
			ptrCode[i].feature = 'R';
		    }
                    break;
                }
            }
	    if (addressMode == 2)
		free(tempWord);		
        }
	else
	    free(tempWord);
    }
}
