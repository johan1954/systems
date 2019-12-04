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
    argumentList *next;
} argumentList;

argumentList *allocate(argumentList *argsPtr, char *argument);
argumentList *freeMemory(argumentList *argsPtr);
argumentList *options(argumentList *argsPtr);
argumentList *changeDirectory(argumentList *currPtr);

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
            printf("This is continue\n");
            continue;
        }
        strtok(command, "\n");
        argument = strtok(command, " ");
        while (argument != NULL) {
            argsPtr = allocate(argsPtr, argument);
            argument = strtok(NULL, " ");
        }
        count = getLength(argsPtr);
        for (int i = 0; i < count; i++) {
            /*TODO: options()*/
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
    while (ptr != NULL) {
        argsPtr = ptr->next;
        if (strcncmp(argsPtr->argument, "&", 1) == 0) {
            count++;
        }
    }
    return count;
}

argumentList *options(argumentList *argsPtr) {
    argumentList *currPtr = argsPtr;


    if (strncmp(argsPtr->argument, "exit", MAX) == 0) {
        exit(0);
    }

    else if (strncmp(argsPtr->argument, "cd", MAX) == 0) {
        currPtr = argsPtr->next;
        return changeDirectory(currPtr);
    }

    else if (strncmp(argsPtr->argument, "path", MAX) == 0) {
        /* Do this too */
    }
    else if ((argsPtr = run_command(argsPtr->argument, argsPtr) != NULL)) {

    }

    else {
        printf("wish: %s: command not found\n", argsPtr->argument);
    }
    return argsPtr;
}

argumentList *changeDirectory(argumentList *currPtr) {
    if (chdir(currPtr->argument) != 0) {
        /*TODO: Error*/
    }
    currPtr = currPtr->next;
    return currPtr;
}



void redirect(char filename[], char writable[]) {
    FILE *file;
    if ((file = fopen(filename, "w")) == NULL) {
        perror("Redirection failed.\n");
        return;
    }
    fprintf(file, writable);


}

void addPath(char *newPath) {
    FILE *file;
    if ((file = fopen("path", "a")) == NULL) {
        perror("Redirection failed.\n");
        return;
    }
    fprintf("%s\n", newPath);
}

argumentList *runCommand(argumentList *argsPtr, int numOfArgs) {
    FILE *file;
    char *path = NULL;
    size_t size = 0;
    if ((file = fopen("path", "r")) == NULL) {
        perror("wish: %s: Path could not be read.\n");
        return;
    }
    while (getline(&path, &size, file)) {
        strcat(path, "/");
        strcat(path, argsPtr->argument);
        if (access(path, X_OK) == 0) {
            break;
        }
        else {
            continue;
        }
        
    }
    pid_t child_process = fork();
    if (child_process == 0) {
        if (execv(path, argsPtr) != NULL) {
                printf();
        }
    }
    else if (child_process < 0) {

    }
    
}

/* eof **********************************************************************************************/