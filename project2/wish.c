/***************************************************************************************************/
/**** Tomi Enberg 0518456 Markus Strandman 0521605 *************************************************/
/**** CT30A3370 Käyttöjärjestelmät ja systeemiohjelmointi ******************************************/
/**** Lähteet: *************************************************************************************/
/**** https://www.educative.io/edpresso/splitting-a-string-using-strtok-in-c ***********************/
/***************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
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
int getLength(argumentList *argsPtr);
int getArgLength(argumentList *argsPtr, int isCommandInString);
void addPath(char *newPath);
argumentList *runCommand(argumentList *argsPtr);

int main(int argc, char *argv[]) {
    char *command = NULL, *argument;
    argumentList *argsPtr = NULL;
    long commandLength;
    size_t size = 0;
    int count;
    if (argc > 1) {
        /* Koodaa toiminnallisuus tähän */
    }
    
    while (TRUE) {
        printf("wish> ");
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
        count = getLength(argsPtr);
        /* Runs as many times as there are different commands separated by "&" */
        for (int i = 0; i < count; i++) {
            argsPtr = options(argsPtr);
        }
    }
    return 0;
}

/* This function allocates memory for incoming commands. Makes it easier to execute in order. */
argumentList *allocate(argumentList *argsPtr, char *argument) {
    if (argument == 0) {
        return NULL;
    }
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

    if ((strcmp(argsPtr->argument, "&") == 0) || (strcmp(argsPtr->argument, ">") == 0)) {
        currPtr = argsPtr->next;
        free(argsPtr);
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
    char path[MAX], argList[MAX] = " ";
    int argsCount, loop;
    
    /* Opens the file where shell paths are stored. */
    if ((file = fopen("path", "r")) == NULL) {
        perror("wish: %s: Path could not be read.\n");
        return NULL;
    }
    while (fgets(path, MAX-1, file) != NULL) {
        strcat(path, "/");
        strcat(path, argsPtr->argument);
        if (access(path, X_OK) == 0) {
            break;
        }
        strcpy(path, "");
    }
    if (strcmp(path, "")) {
        printf("wish: %s: command not found\n", argsPtr->argument);
        return NULL;
    }
    argsCount = getArgLength(argsPtr,1);
    argv[MAX];
    for (loop = 0; loop < argsCount; loop++) {
        currPtr = currPtr->next;
        strcat(argList, currPtr->argument);
        argv[loop] = currPtr->argument;
        strcat(argList, " ");
    }

    fclose(file);
    printf("%s", path);
    pid_t child_process = fork();
    if (child_process == 0) {
        if (execv(path, argv) == -1) {
            printf(": %s: error while running system command at path\n", path);
            perror("\n");
            return NULL;
        }
    }
    else if (child_process < 0) {
        printf(": %s: error while creating new process\n", path);
        perror("\n");
        return NULL;
    }
    return currPtr->next;
}

/* eof **********************************************************************************************/