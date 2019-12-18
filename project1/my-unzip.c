#include <stdio.h>
#include <stdlib.h>
#define TRUE 1

int main(int argc, char* argv[]) {
    int count, x = 0;
    char new_char;
    FILE *file;
    /*If there is no filename in arguments, return 1*/
    if (argc <= 1) {
        printf("my-unzip: file1 [file2 ...]\n");
        return 1;
    }
    /*Open file for reading*/
    if ((file = fopen(argv[1], "r")) == NULL) {
        perror("Cannot open file\n");
        exit(1);
    }
    while (TRUE) {
        /*Read how many times the next letter occurs in a row*/
        if(fread(&count, sizeof(int), 1, file) == 0){
            break;
        }
        /*Read what is the next letter*/
        fread(&new_char, sizeof(char), 1, file);
        /*Print that letter count times*/
        for (x = 0; x < count; x++) {
            printf("%c",new_char);
        }
    }
    printf("\n");
    return 0;
}