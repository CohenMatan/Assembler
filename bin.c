#include "tables.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#define BinarySize 12

/*Converts positive number or negative number to binary number in size 12 */
char *numToBin(int num){
    char arrToCpy[12];
    char *binary = "";
    int i;
    int ifNegative = num;

    for(i=0;i<12;i++){
        arrToCpy[i] = '0';
    }

    i = 11;
    while (num != 0){
        if (num%2 == 1 || num%2 == -1)
            arrToCpy[i] = '1';
        else
            arrToCpy[i] = '0';
        num /= 2;
        i--;
    }

    if (ifNegative < 0){
        for(i=11;i>=0;i--){
            if (arrToCpy[i] == '1')
                arrToCpy[i] = '0';
            else
                arrToCpy[i] = '1';
        }
        for(i=11;i>=0;i--){
            if(arrToCpy[i] == '1')
                arrToCpy[i] = '0';
            else {
                arrToCpy[i] = '1';
                break;
            }
        }
    }

    binary = strdup(arrToCpy);
    binary[12] = '\0';
    return binary;
}


/*Encodes the arguments coming from a instruction line and enters data to the code table.*/
void argusToBin(int addressing,  char *argu){
    extern int IC;
    int i;
    int arrIndex = 11;
    int num = 0;
    char *binary = "";
    char arrToCpy[BinarySize];
    
    for(i=0;i<BinarySize;i++){/* Initialize arrToCpy */
        arrToCpy[i] = '\0';
    }

    if(addressing == 0){/* immediate */
        for (i=0;i<strlen(argu);i++){
            arrToCpy[i] = argu[i+1];
        }
        arrToCpy[i] = '\0';
        num = atoi(arrToCpy);
        binary = numToBin(num);
        ptrCode[IC-100].line = IC;
        ptrCode[IC-100].binStr = binary;
        ptrCode[IC-100].feature = 'A';
    }
    else if(addressing == 1){/* direct */
        ptrCode[IC-100].line = IC;
        ptrCode[IC-100].binStr = "?";
        ptrCode[IC-100].feature = '?';
    }
    else if(addressing == 2){/* relative */
        ptrCode[IC-100].line = IC;
        ptrCode[IC-100].binStr = "?";
        ptrCode[IC-100].feature = 'A';
    }
    else if(addressing == 3){/* register direct */
        arrToCpy[0] = argu[1];
        num = atoi(arrToCpy);
        for(i=0;i<12;i++){
            if (i == num)
                arrToCpy[arrIndex-i] = '1';
            else
                arrToCpy[arrIndex-i] = '0';
        }

        binary=strdup(arrToCpy);
        binary[12] = '\0';
        ptrCode[IC-100].line = IC;
        ptrCode[IC-100].binStr = binary;
        ptrCode[IC-100].feature = 'A';
    }
}



/*Convert the source code (the line) to binary code and returns it to put it in the table.*/
char *sourceToBin(int arrIndex, int firstAddressing, int secAddressing) {
    char *binary = "";
    char arrToCpy[BinarySize];
    int i = 0;
    int k = 10;

    while(i < 4){
        arrToCpy[i] = actionTable(arrIndex,0,i);
        i++;
    }

    while(i < 8){
        arrToCpy[i] = actionTable(arrIndex,1,i);
        i++;
    }

    if(firstAddressing != -1 && secAddressing == -1){
        while(i < 10){
            arrToCpy[i] = '0';
            k++;
            i++;
        }
        k = 10;
        binary = numToBin(firstAddressing);
        while(i < 12){
            arrToCpy[i] = binary[k];
            k++;
            i++;
        }
    }
    else if(firstAddressing != -1){
        binary = numToBin(firstAddressing);
        while(i < 10){
            arrToCpy[i] = binary[k];
            k++;
            i++;
        }
    }
    else
        while(i < 10){
            arrToCpy[i] = '0';
            i++;
        }

    k=10;
    if(secAddressing != -1){
        if(*binary != '\0')
	    free(binary);
        binary = numToBin(secAddressing);
        while(i < 12){
            arrToCpy[i] = binary[k];
            k++;
            i++;
        }
    }
    else
        while(i < 12){
            arrToCpy[i] = '0';
            i++;
        }

    if(*binary != '\0')
	 free(binary);
    binary=strdup(arrToCpy);
    binary[12] = '\0';
    return binary;
}


/*Converts binary number to decimal number.*/
unsigned long  binToNum( unsigned long n) {
    int dec = 0;
    int i = 0;
    int rem;
    
    while (n != 0) {
        rem = n % 10;
        n /= 10;
        dec += rem*pow(2,i);
        ++i;
    }
    return dec;
}
