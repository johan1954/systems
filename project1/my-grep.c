#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define StaticBuffer 256

void file_open(char* filename, char* search);
void read_stdin(char* search);

int main(int argc, char* argv[]) {
    int x;
    if (argc <= 1) {
        perror("my-grep: searchterm [file ...]\n");
        exit(1);
    }
    else if (argc == 2) {
        read_stdin(argv[1]);
        return 0;
    }
    else {
        for (x = 0; x < argc-2; x++) {
            file_open(argv[1], argv[2+x]);
        }
    }
    return 0;
}

/* Main functionality in this function */
void file_open(char* search, char* filename) {
    char *buffer = NULL, *ptr;
    size_t size = 0;
    FILE *file1;

    if ((file1 = fopen(filename, "r")) == NULL) {
        perror("my-grep: cannot open file\n");
        exit(1);
    }
    while (getline(&buffer, &size, file1) != -1) {
        if ((ptr = strstr(buffer, search)) != NULL) {
            printf("%s", buffer);
        }
    }
    if (buffer != NULL) {
        free(buffer);
        buffer = NULL;
    }
    printf("\n");
}

/* If no file has been given, only reading stdin. */
void read_stdin(char* search) {
    char stdinBuffer[StaticBuffer];

    if ((fgets(stdinBuffer, StaticBuffer-1, stdin)) == NULL) {
        return;
    }

    if (strstr(stdinBuffer, search) != NULL) {
        printf("%s", stdinBuffer);
    }
}