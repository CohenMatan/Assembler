#include <stdio.h>
#include <stdlib.h>

void print_error(int index,int line,char *nameOfFile){
    enum error{error19=-19,error18,error17,error16,error15,error14,error13,error12,error11,error10,error9,error8,error7,error6,error5,error4,error3,error2,error1};

    switch (index) {
        case error1:{
            printf("ERROR : PLEASE ENTER FILE NAME.\n");
            exit(0);
        }
        case error2:
            printf("ERROR : CANNOT OPEN FILE \"%s\".\n",nameOfFile);
            break;
        case error3:
            printf("ERROR : IN FILE \"%s\" LINE(%d): LABEL IS RESERVED WORD.\n",nameOfFile,line);
            break;
        case error4:
            printf("ERROR : IN FILE \"%s\" LINE(%d): INVALID LABEL.\n",nameOfFile,line);
            break;
        case error5:
            printf("ERROR : IN FILE \"%s\" LINE(%d): ARGUMENTS SYNTAX ERROR.\n",nameOfFile,line);
            break;
        case error6:
            printf("ERROR : IN FILE \"%s\" LINE(%d): MISSING ARGUMENT.\n",nameOfFile,line);
            break;
        case error7:
            printf("ERROR : IN FILE \"%s\" LINE(%d): INVALID SENTENCE.\n",nameOfFile,line);
            break;
        case error8:
            printf("ERROR : IN FILE \"%s\" LINE(%d): TOO MANY ARGUMENTS.\n",nameOfFile,line);
            break;
        case error9:
            printf("ERROR : IN FILE \"%s\" LINE(%d): ADDRESSING MODE DO NOT MATCH TO THE INSTRUCTION WORD.\n",nameOfFile,line);
            break;
        case error10:
            printf("WARNING : IN FILE \"%s\" LINE(%d): LABEL IN ENTRY COMMEND.\n",nameOfFile,line);
            break;
        case error11:
            printf("ERROR : IN FILE \"%s\" LINE(%d): LABEL IS ALREADY DEFINED.\n",nameOfFile,line);
            break;
        case error12:
            printf("ERROR : IN FILE \"%s\" LINE(%d): LABEL IS NOT LEGAL.\n",nameOfFile,line);
            break;
        case error13:{
            printf("MEMORY ALLOCATION FAILED.\n");
            exit(0);
        }
        case error14:
            printf("ERROR : IN FILE \"%s\" LINE(%d): CANNOT FIND LABEL AT ENTRY COMMEND.\n",nameOfFile,line);
            break;
        case error15:
            printf("ERROR : IN FILE \"%s\" LINE(%d): THE LABEL AT THE ARGUMENT NOT DEFINED.\n",nameOfFile,line);
            break;
        case error16:{
            printf("ERROR : IN FILE \"%s\" : STORAGE SIZE HAS PASSED ITS QUOTA.\n",nameOfFile);
            exit(0);
        }
        case error17:
            printf("ERROR : IN FILE \"%s\" LINE(%d): NUMBER OUT OF BOUNDS.\n",nameOfFile,line);
            break;
        case error18: {
            printf("ERROR : CANNOT CLOSE FILE \"%s\".\n",nameOfFile);
            exit(0);
        }
	case error19:
            printf("ERROR : IN FILE \"%s\" LINE(%d): LINE IS TOO LONG.\n",nameOfFile,line);
            break;
    }
}
