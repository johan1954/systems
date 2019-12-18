/***************************************************************************************************/
/**** Tomi Enberg 0518456 Markus Strandman 0521605 *************************************************/
/**** CT30A3370 Käyttöjärjestelmät ja systeemiohjelmointi ******************************************/
/**** Lähteet: *************************************************************************************/
/**** https://www.educative.io/edpresso/splitting-a-string-using-strtok-in-c
 ****   https://www.geeksforgeeks.org/wait-system-call-c/ ***********************/
/***************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#define TRUE 1
#define MAX 100
#define ARGVL 20

typedef struct ArgumentList {
    char argument[MAX];
    struct ArgumentList *next;
} argumentList;

typedef struct Argument {
    char string[MAX];
} argument;

argumentList *allocate(argumentList *argsPtr, char *argument);
argumentList *freeMemory(argumentList *argsPtr);
argumentList *options(argumentList *argsPtr);
argumentList *changeDirectory(argumentList *currPtr);
int getLength(argumentList *argsPtr);
int getArgLength(argumentList *argsPtr, int isCommandInString);
void addPath(char *newPath);
argumentList *runCommand(argumentList *argsPtr);

int main(int argc, char *argv[]) {
    char *command = NULL, *argument;
    argumentList *argsPtr = NULL, *currPtr = NULL;
    long commandLength;
    size_t size = 0;
    int count;
    if (argc > 1) {
        /* Koodaa toiminnallisuus tähän */
    }
    /* The shell interface */
    while (TRUE) {
        printf("wish> ");
        /* Get the string, arbirary length allowed. */
        commandLength = getline(&command, &size, stdin);
        if (commandLength == 1) {
            /* printf("This is continue\n"); */
            continue;
        }
        printf("%s", command);
        strtok(command, "\n");
        argument = strtok(command, " ");
        if (command == 0) {
            continue;
        }
        while (argument != NULL) {
            argsPtr = allocate(argsPtr, argument);
            argument = strtok(NULL, " ");
        }
        /* Runs as many times as there are different commands */
        currPtr = argsPtr;
        while (currPtr != NULL) {
            currPtr = options(currPtr);
            if (currPtr == NULL) {
                break;
            }
            if (strcmp(currPtr->argument, "&") == 0) {
                currPtr = currPtr->next;
                continue;
            }
            /* Redirect handled in the execv -child process. */
            /* If there is an error in the redirect, it will also be handled before reaching this point */
            else if (strcmp(currPtr->argument, ">") == 0){
                break;
            }
        }
        if (argsPtr != NULL) {
            argsPtr = freeMemory(argsPtr);
        }
    }
    return 0;
}

/* This function allocates memory for incoming commands. Makes it easier to execute in order. */
argumentList *allocate(argumentList *argsPtr, char *argument) { 
    argumentList *newArgsPtr, *currArgsPtr = argsPtr;
    if ((newArgsPtr = (argumentList *)malloc(sizeof(argumentList))) == NULL) {
        perror("Allocating memory failed.\n");
        exit(1);
    }
    strcpy(newArgsPtr->argument, argument);
    newArgsPtr->next = NULL;
    if (argsPtr == NULL) {
        return newArgsPtr;
    }
    while (currArgsPtr->next != NULL) {
        currArgsPtr = currArgsPtr->next;
    }
    currArgsPtr->next = newArgsPtr;
    return argsPtr;
}


argumentList *freeMemory(argumentList *argsPtr) {
    argumentList *ptr;
    ptr = argsPtr;
    while (ptr != NULL) {
        argsPtr = ptr->next;
        free(ptr);
        ptr = argsPtr;
    }
    return NULL;
}

int getLength(argumentList *argsPtr) {
    argumentList *ptr;
    ptr = argsPtr;
    int count = 1;
    while (ptr->next != NULL) {
        argsPtr = ptr->next;
        if (strncmp(argsPtr->argument, "&", 1) == 0) {
            count++;
        }
    }
    return count;
}

int getArgLength(argumentList *argsPtr, int isCommandInString) {
    argumentList *ptr;
    ptr = argsPtr;
    int count = 1;
    if (argsPtr == NULL) {
        return 0;
    }
    if (isCommandInString == 1) {
        isCommandInString = 0;
        count = 0;
    }

    while (ptr->next != NULL) {
        
        argsPtr = ptr->next;
        if ((strncmp(argsPtr->argument, "&", 1) == 0) || (strncmp(argsPtr->argument, ">", 1) == 0)) {
            break;
        }
        else {
            count++;
        }
    }
    return count;
}

argumentList *options(argumentList *argsPtr) {
    argumentList *currPtr = argsPtr;
    printf("Options subroutine reached!\n");

    if ((strcmp(argsPtr->argument, "&") == 0) || (strcmp(argsPtr->argument, ">") == 0)) {
        currPtr = argsPtr->next;
    }
    if (strncmp(currPtr->argument, "exit", MAX) == 0) {
        exit(0);
    }

    else if (strncmp(currPtr->argument, "cd", MAX) == 0) {
        currPtr = changeDirectory(currPtr);
        if (currPtr == NULL) {
            return NULL;
        }
    }

    else if (strncmp(currPtr->argument, "path", MAX) == 0) {
        /* Do this too */
    }
    else {
        currPtr = runCommand(currPtr);
    }
    return currPtr;
}

argumentList *changeDirectory(argumentList *argsPtr) {
    argumentList *currPtr = argsPtr->next;
    free(argsPtr);
    if (getArgLength(currPtr, 0) < 1) {
        printf("cd without argument.\n");
        return NULL;
    }
    else if (getArgLength(currPtr, 0) > 1) {
        printf("cd with too many arguments.\n");
        return NULL;
    }
    if (chdir(currPtr->argument) != 0) {
        /*TODO: Error*/
    }
    else {
        return NULL;
    }
    return currPtr;
}



void redirect(char filename[], char writable[]) {
    FILE *file;
    if ((file = fopen(filename, "w")) == NULL) {
        perror("Redirection failed.\n");
        return;
    }
    fprintf(file, "%s", writable);
    fclose(file);


}

void addPath(char *newPath) {
    FILE *file;
    if ((file = fopen("path", "a")) == NULL) {
        perror("Redirection failed.\n");
        return;
    }
    fprintf(file, "%s\n", newPath);
    fclose(file);
}

/* Runs a system command */
argumentList *runCommand(argumentList *argsPtr) {
    argumentList *currPtr = argsPtr;
    FILE *file;
    char path[MAX], tempCommand[MAX] = "", filename[MAX] = "";
    char *argv[] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    int loop = 1, redirectInt = 0;
    
    /* Opens the file where shell paths are stored. */
    if ((file = fopen("path", "r")) == NULL) {
        perror("wish: %s: Path could not be read.\n");
        return NULL;
    }

    while (fgets(path, MAX-1, file) != NULL) {
        strtok(path, "\n");
        strcat(path, "/");
        strcpy(tempCommand, argsPtr->argument);
        strcat(path, tempCommand);
        printf("%s", path);
        if (access(path, X_OK) == 0) {
            printf("Access ok!\n");
            break;
        }
        strcpy(path, "");
    }
    fclose(file);

    if (strcmp(path, "") == 0) {
        printf("wish: %s: command not found\n", argsPtr->argument);
        return NULL;
    }
    argv[0] = currPtr->argument;
    currPtr = currPtr->next;
    while (currPtr != NULL) {
        if (strcmp(currPtr->argument, "&") == 0) {
            break;
        }
        else if (strcmp(currPtr->argument, ">") == 0) {
            strcpy(filename, currPtr->next->argument);
            if (strcmp(filename, "") == 0) {
                printf("wish: execv: error while defining filename");
                return currPtr;
            }            
            redirectInt = 1;
            break;
        }
        argv[loop] = currPtr->argument;
        loop++;
        currPtr = currPtr->next;
    }
 
    //printf("%s", path);
    pid_t child_process = fork();

    if (child_process < 0) {
        /* Error */
        perror("There is something wrong with fork!\n");
        exit(-1);
    }
    else if (child_process != 0) {
        /* Parent process */
        printf("Waiting child\n");
        wait(NULL);
        printf("Waiting over\n");
    }
    else {
        /* Child process, here we execute the execv */
        /* ARGVL is defined in the scope, change if necessary */
        //printf("Let's execute!\n");
        if (redirectInt == 1) {
            if ((file = freopen(filename, "w", stdout)) == NULL) {
                perror("wish: execv: Cannot write to file");
                exit(-1);
            }
        }
        
        if (execv(path, argv) == -1) {
            if (redirectInt == 1) {
                fclose(file);
            }
            perror("Execv failed while running!");
            exit(-1);
        }
        if (redirectInt == 1) {
                fclose(file);
        }
    }
    return currPtr;
}

/* eof **********************************************************************************************/