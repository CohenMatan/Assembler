#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "passages.h"
#include "utils.h"
#include "errors.h"
#include "textCheck.h"
#include "files.h"

char *strdup(const char *);

int IC=100;
int DC=0;

int main(int argc, char *argv[]) {
    FILE * fd; /*For the assembly file*/
    enum result {NO_ERROR,ERROR};
    int result1 = NO_ERROR;
    int result2 = NO_ERROR;
    int index = 1;
    int *errorNum;
    int varErrNum = 0;
    char *nameOfFile = "";
    char *typeOfFile = ".as";

    errorNum = &varErrNum;

    if(argc == 1){ /*No argument enterd. */
        *errorNum = -1;
        print_error(*errorNum,0,"");
    }

    while(index != argc){ /*Loop for all the entered arguments */
        nameOfFile = strdup(argv[index]);
        nameOfFile = (char*)realloc(nameOfFile,strlen(nameOfFile)+4);
        nameOfFile = strcat(nameOfFile, typeOfFile);

        if(!(fd=fopen(nameOfFile,"r"))) { /*If the file is not found */
            *errorNum = -2;
            print_error(*errorNum,0,nameOfFile);
	    *errorNum = 0;
	    index++;
	    free(nameOfFile);
	    continue;
        }

        result1=passage1(fd,nameOfFile,errorNum); /*Starting the first passage */

        if (result1==NO_ERROR)
            result2=passage2(fd,nameOfFile,errorNum); /*Countinues to the second passage*/

        if (result1==NO_ERROR && result2==NO_ERROR){/* Creating files */
            createOb(argv[index],errorNum);
            if (ptrSymbol != NULL){
                createExt(argv[index],errorNum);
                createEnt(argv[index],errorNum);
            }
        }

        if( index < argc){/*Reset the structures.*/
      	    structReset();
        }

	if(fclose(fd)){
            *errorNum=-18;
            print_error(*errorNum,0,nameOfFile);
        }

        IC=100;
        DC=0;
        result1=NO_ERROR;
        result2=NO_ERROR;
        index++;
	free(nameOfFile);
    }       
    return 0;
}
