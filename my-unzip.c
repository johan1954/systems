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
    if ((file = fopen(argv[1], "r")) == NULL) {
        perror("Cannot open file\n");
        exit(1);
    }

    fread(buffer, 1, sizeof(object), file);
    printf("%s", buffer);
    return 0;
}