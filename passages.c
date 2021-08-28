#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bin.h"
#include "utils.h"
#include "errors.h"
#include "textCheck.h"
#define maxCharsInLine 81
#define maxStorage 4096


int passage1(FILE *fd,char *nameOfFile,int *errorNum) {
    extern int IC;
    extern int DC;
    enum ErrorStatus {
        NO_ERROR, ERROR
    };
    int i = 0;
    int c;
    int codeStorageSize;
    int flag = NO_ERROR;
    int lineNumber = 1;
    char buf[maxCharsInLine+1];

    /*Read line by line from the assembly file and send it to function to check syntax. If error found, the errorFlag=-1.*/
    while ((fgets(buf, maxCharsInLine+1, fd)) != NULL) {/*+1 to check if the line in too long at the function "firstWord". */
        firstWord(buf, errorNum);
        if (*errorNum <= -1) {
            if (*errorNum != -10)
                flag = ERROR;
	    if(*errorNum == -19) {
		while ((c = getc(fd))) {
		    if(c == '\n' || c == EOF)
			break;     		
		}
  	    }
            print_error(*errorNum, lineNumber, nameOfFile); /* If error found */
        }
        lineNumber++;
        *errorNum = 0;
    }
    if(IC == 100 && DC == 0) /*If the file is empty*/
 	return 0;

    if((DC+IC-100)>maxStorage) 
        print_error(-16,0,nameOfFile);

    /*Merging the data table and the code table to one table */
    codeStorageSize = IC-100;
    ptrCode = (struct sCode *) realloc(ptrCode, ((DC + codeStorageSize) * sizeof(struct sCode)));
    if (!ptrCode) {
        *errorNum = -13;
        print_error(*errorNum, lineNumber, nameOfFile);
    } else {
        while (i < DC) {
            ptrCode[codeStorageSize].line = ptrData[i].line + IC;
            ptrCode[codeStorageSize].binStr = ptrData[i].binStr;
            ptrCode[codeStorageSize].feature = ptrData[i].feature;
            i++;
            if (i != DC)
                codeStorageSize++;
        }
    }
   
    if (flag == 0)
        return 0;
    return 1;
}

int passage2(FILE *fd,char *nameOfFile,int *errorNum){
    extern int IC;
    extern int DC;
    enum ErrorStatus {NO_ERROR, ERROR};
    int errorFlag = 0;
    int *bufIndex;
    int symbolStorageIndex;
    int numLine = 1;
    int varBuffNum = 0;
    int i = 0; 
    char buf[maxCharsInLine];
    bufIndex = &varBuffNum;
    rewind(fd);

    if (ptrSymbol != NULL){
        symbolStorageIndex=ptrSymbol[0].numOfSymbols-2;
        while ( i < ptrSymbol[0].numOfSymbols-1){ /* Update the num line of the data symbols. */
            if (!strcmp(ptrSymbol[i].attributes,"data"))
                ptrSymbol[i].value += IC;
            i++;
        }
    }

    while ((fgets(buf, maxCharsInLine, fd)) != NULL){/*Update the code table*/
        codeUpdate(buf,bufIndex,errorNum);
        if(*errorNum != 0 && *errorNum != -5 && *errorNum != -12){
            print_error(*errorNum,numLine,nameOfFile);
            errorFlag = 1;
        }
        numLine++;
        *bufIndex = 0;
        *errorNum = 0;
    }

    rewind(fd);
    numLine = 1;
    while ((fgets(buf, maxCharsInLine, fd)) != NULL){/*Update the symbol table*/
        symbolUpdate(buf,bufIndex,errorNum,symbolStorageIndex);
        if(*errorNum != 0 && *errorNum != -12){
            print_error(*errorNum,numLine,nameOfFile);
            errorFlag = 1;
        }
        numLine++;
        *bufIndex = 0;
        *errorNum = 0;
    }  
    return errorFlag;
}
