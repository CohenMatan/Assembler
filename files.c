#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tables.h"
#include "errors.h"
#include "bin.h"

char *itoa(int value,char *str,int base);

extern int IC;
extern int DC;

/*Creates the ob file.*/
void createOb(char *nameOfFile,int *errorNum){
    FILE * fdOb;
    int i;
    int k = 0;
    int j = 0;
    int h = 0;
    int codeStorageSize;
    unsigned long num = 0;
    char str[20];
    char *typeOfFile = ".ob";
    char *cpyName = "";
    codeStorageSize = IC-100;

    cpyName = strdup(nameOfFile);
    cpyName = (char*)realloc(cpyName,4+strlen(cpyName));
    cpyName = strcat(cpyName,typeOfFile);
    fdOb = fopen(cpyName,"w");

    itoa(codeStorageSize, str, 10);
    fprintf(fdOb,"\t%s ",str);
    fprintf(fdOb,"%d\n",DC);

    for(i=0; i < codeStorageSize + DC; i++){ /* .ob file */
        /*Copy the number of the line to the new file*/
        fprintf(fdOb,"0%d ",ptrCode[i].line);
        /*"str" gets 4 characters 4 times from the binStr and covert it to HEX char.*/
        while(j < 15) {
            if (j!=4 && j!=9 && j!=14){
                str[k] = ptrCode[i].binStr[h];
                k++;
                h++;
            }
            else{
                str[k] = '\0';
                num = atof(str);
                num = binToNum(num);
                itoa((int)num,str,16);
                fprintf(fdOb,"%s",str);
                k = 0;
            }
            j++;
        }
        /*Copy the feature of the line to the new file*/
        fprintf(fdOb," %c\n",ptrCode[i].feature);
        j = 0;
        h = 0;
    }

    if(fclose(fdOb)){
        *errorNum=-18;
        print_error(*errorNum,0,nameOfFile);
    }
    if(*cpyName != '\0')
	free(cpyName);
}


/*Creates the ext file.*/
void createExt(char *nameOfFile,int *errorNum){
    FILE * fdExt;
    int i = 0;
    int k = 0;
    char *typeOfFile = ".ext";
    char *cpyName = "";

    while(i != ptrSymbol[0].numOfSymbols-1){/*Checks if there is external label at the symbol table*/
        if(!strcmp(ptrSymbol[i].attributes,"external")){
            cpyName = strdup(nameOfFile);
            cpyName = (char*)realloc(cpyName,5+strlen(cpyName));
            cpyName = strcat(cpyName,typeOfFile);
            fdExt = fopen(cpyName,"w");
            break;
        }
        i++;
    }
    if(i < ptrSymbol[0].numOfSymbols-1){
        for(i=0;i<DC+IC-100;i++){
            if(ptrCode[i].feature == 'E'){
                while (k < ptrSymbol[0].numOfSymbols-1){
                    if(!strcmp(ptrCode[i].externWord,ptrSymbol[k].label)){
                        fprintf(fdExt,"%s 0%d\n",ptrSymbol[k].label,ptrCode[i].line);
                        k = 0;
                        break;
                    }
                    k++;
                }
            }
        }
    }

    if(fclose(fdExt)){
        *errorNum=-18;
        print_error(*errorNum,0,cpyName);
    }
    if(*cpyName != '\0')
	free(cpyName);
}


/*Creates the ent file.*/
void createEnt(char *nameOfFile,int *errorNum){
    FILE * fdEnt;
    int i = 0;
    char *typeOfFile = ".ent";
    char *cpyName = "";

    while(i != ptrSymbol[0].numOfSymbols-1){/*Checks if there is entry CheckLabel at the symbol table*/
        if(!strcmp(ptrSymbol[i].attributes,"data, entry") || !strcmp(ptrSymbol[i].attributes,"code, entry")){
            cpyName = strdup(nameOfFile);
	    cpyName = (char*)realloc(cpyName,5+strlen(cpyName));
            cpyName = strcat(cpyName,typeOfFile);
            fdEnt = fopen(cpyName,"w");
            break;
        }
        i++;
    }
    if(i < ptrSymbol[0].numOfSymbols-1){
        while(i != ptrSymbol[0].numOfSymbols-1){
            if(!strcmp(ptrSymbol[i].attributes,"data, entry") || !strcmp(ptrSymbol[i].attributes,"code, entry")){
                fprintf(fdEnt,"%s 0%d\n",ptrSymbol[i].label,ptrSymbol[i].value);
            }
            i++;
        }
    }

    if(fclose(fdEnt)){
        *errorNum=-18;
        print_error(*errorNum,0,cpyName);
    }
    if(*cpyName != '\0')
	free(cpyName);
}
