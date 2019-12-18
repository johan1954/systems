/***************************************************************************************************/
/**** Tomi Enberg 0518456 Markus Strandman 0521605 *************************************************/
/**** CT30A3370 Käyttöjärjestelmät ja systeemiohjelmointi ******************************************/
/**** Lähteet: *************************************************************************************/
/**** https://www.educative.io/edpresso/splitting-a-string-using-strtok-in-c ***********************/
/****   https://www.geeksforgeeks.org/wait-system-call-c/ ******************************************/
/***************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#define TRUE 1
#define MAX 100

typedef struct ArgumentList {
    char argument[MAX];
    struct ArgumentList *next;
} argumentList;

argumentList *allocate(argumentList *argsPtr, char *argument);
argumentList *freeMemory(argumentList *argsPtr);
argumentList *options(argumentList *argsPtr);
argumentList *changeDirectory(argumentList *currPtr);
argumentList *runCommand(argumentList *argsPtr);
int getArgLength(argumentList *argsPtr, int isCommandInString);
void addPath(char *newPath);
/* Define shell path */
argumentList *pathList = NULL;

int main(int argc, char *argv[]) {
    char *command = NULL, *argument;
    argumentList *argsPtr = NULL, *currPtr = NULL;
    long commandLength;
    size_t size = 0;
    FILE *file;
    /* Initialize shell path */
    pathList = allocate(pathList, "/bin");

    /* If a file has been given as a command line argument */
    if (argc > 1) {
        if ((file = fopen(argv[1], "r")) == NULL) {
            perror("Cannot open file\n");
            exit(1);
        }
        /* Go through the entire file line by line */
        while (TRUE) {
            commandLength = getline(&command, &size, file);
            if (commandLength <= 1) {
                break;
            }
            strtok(command, "\n");
            argument = strtok(command, " ");
            if (command == 0) {
                continue;
            }
            while (argument != NULL) {
                argsPtr = allocate(argsPtr, argument);
                argument = strtok(NULL, " ");
            }
            currPtr = argsPtr;
            /* Run as long as there is a following command on the line */
            while (currPtr != NULL) {
                currPtr = options(currPtr);
                if (currPtr == NULL) {
                    break;
                }
                /* If an argument is an ampersand, skip it and move to the next command */
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
            /* When exiting command list, reset the command list for the next line */
            /* and free up the value that was allocated */
            if (argsPtr != NULL) {
                argsPtr = freeMemory(argsPtr);
            }
        }
        /* Before exiting file mode, free memory */
        if (pathList != NULL) {
            pathList = freeMemory(pathList);
        }
        if (argsPtr != NULL) {
            argsPtr = freeMemory(argsPtr);
        }
        return 0;
    }
    /* The shell interface */
    while (TRUE) {
        printf("wish> ");
        /* Get the string, arbitrary length allowed. */
        commandLength = getline(&command, &size, stdin);
        if (commandLength == 1) {
            continue;
        }
        /* Split the given command string into commands and arguments */
        strtok(command, "\n");
        argument = strtok(command, " ");
        if (command == 0) {
            continue;
        }
        while (argument != NULL) {
            argsPtr = allocate(argsPtr, argument);
            argument = strtok(NULL, " ");
        }
        currPtr = argsPtr;
        /* Run as long as there is a following command in the list */
        while (currPtr != NULL) {
            currPtr = options(currPtr);
            if (currPtr == NULL) {
                break;
            }
            /* If an argument is an ampersand, skip it and move to the next command */
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
        /* When exiting command list, reset the command list for new inputs */
        /* and free up the value that was allocated */
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

/* Get the argument count from the given list */
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

/* Menu for given commands */
argumentList *options(argumentList *argsPtr) {
    argumentList *currPtr = argsPtr;

    if ((strcmp(argsPtr->argument, "&") == 0) || (strcmp(argsPtr->argument, ">") == 0)) {
        currPtr = argsPtr->next;
    }
    if (strncmp(currPtr->argument, "exit", MAX) == 0) {
        /* Before exiting, free the allocated memory from both dynamic data-structures */
        if (pathList != NULL) {
            pathList = freeMemory(pathList);
        }
        if (argsPtr != NULL) {
            argsPtr = freeMemory(argsPtr);
        }
        exit(0);
    }

    else if (strncmp(currPtr->argument, "cd", MAX) == 0) {
        currPtr = changeDirectory(currPtr);
        if (currPtr == NULL) {
            return NULL;
        }
    }

    else if (strncmp(currPtr->argument, "path", MAX) == 0) {
        pathList = freeMemory(pathList);
        currPtr = currPtr->next;
        while (currPtr != NULL) {
            pathList = allocate(pathList, currPtr->argument);
            currPtr = currPtr->next;
        }
        return NULL;
    }
    else {
        currPtr = runCommand(currPtr);
    }
    return currPtr;
}

argumentList *changeDirectory(argumentList *argsPtr) {
    argumentList *currPtr = argsPtr->next;
    if (getArgLength(currPtr, 0) < 1) {
        printf("cd without argument.\n");
        return NULL;
    }
    else if (getArgLength(currPtr, 0) > 1) {
        printf("cd with too many arguments.\n");
        return NULL;
    }
    if (chdir(currPtr->argument) != 0) {
        printf("Permission to open the directory is denied\n");
        return NULL;
    }
    else {
        return NULL;
    }
    return currPtr;
}

/* Runs a system command */
argumentList *runCommand(argumentList *argsPtr) {
    argumentList *pathPtr = pathList;
    argumentList *currPtr = argsPtr;
    FILE *file;
    char filename[MAX] = "", path[MAX] = "";
    /* Define an argument vector. Static, not very safe */
    char *argv[] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    int loop = 1, redirectInt = 0;

    /* Check the access path, and command availability */
    while (pathPtr != NULL) {
        strcpy(path, pathPtr->argument);
        strcat(path, "/");
        strcat(path, argsPtr->argument);
        if (access(path, X_OK) == 0) {
            break;
        }
        pathPtr = pathPtr->next;
        strcpy(path, "");
    }
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
 
    /* Fork a child process */
    pid_t child_process = fork();

    if (child_process < 0) {
        /* Error */
        perror("There is something wrong with fork!\n");
        exit(-1);
    }
    else if (child_process != 0) {
        /* Parent process */
        wait(NULL);
    }
    else {
        /* Child process, here we execute the execv */
        /* redirectInt tells the child process, if a stdout is to be redirected in to a file  */
        if (redirectInt == 1) {
            if ((file = freopen(filename, "w", stdout)) == NULL) {
                perror("wish: execv: Cannot write to file");
                exit(-1);
            }
        }
        /* Error handling, if returns, execv has failed */
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

/* Print linked list, debugging purposes */
// void printLinkedList(argumentList *argsPtr) {
//     argumentList *currPtr = argsPtr;    
//     while (currPtr != NULL) {
//         printf("In the zone!\n");
//         printf("%s\n", currPtr->argument);
//         currPtr = currPtr->next;
//     }
// }
/* eof **********************************************************************************************/