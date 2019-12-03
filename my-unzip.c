#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 900

typedef struct Object{
    int integer;
    char character;
} object;

int main(int argc, char* argv[]) {
    char buffer[MAX];
    FILE *file;
    if (argc <= 1) {
        printf("my-unzip: file1 [file2 ...]\n");
        return 1;
    }
    if ((file = fopen(argv[1], "r")) == NULL) {
        perror("Cannot open file\n");
        exit(1);
    }
    fread(buffer, sizeof(int), 1, file);
    printf("%s", buffer);
    fread(buffer, sizeof(char), 1, file);
    printf("%s", buffer);
    return 0;
}