#include <stdio.h>
#include <stdlib.h>
#define TRUE 1

int main(int argc, char* argv[]) {
/*     char buffer[MAX]; */
    int count, x = 0;
    char new_char;
    FILE *file;
    if (argc <= 1) {
        printf("my-unzip: file1 [file2 ...]\n");
        return 1;
    }
    if ((file = fopen(argv[1], "r")) == NULL) {
        perror("Cannot open file\n");
        exit(1);
    }
    while (TRUE) {
        if(fread(&count, sizeof(int), 1, file) == 0){
            break;
        }
        fread(&new_char, sizeof(char), 1, file);
        for (x = 0; x < count; x++) {
            printf("%c",new_char);
        }
    }
    printf("\n");
    return 0;
}