#include <stdio.h>
#include <stdlib.h>
#define BUFFER_SIZE 1000

/*
Source:
https://moodle.lut.fi/pluginfile.php/401112/mod_resource/content/0/KasurinenNikula2013-CKieli.pdf'
*/

void readFile(char file_name[]);

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        printf("No arguments given.\n");
        exit(0);
    }

    /* Read as many files as there are arguments, reading files in order*/
    for (int argnum = 1; argnum < argc; argnum++) {
        readFile(argv[argnum]);
    }
    return 0;
}

void readFile(char file_name[]) {
    char buffer[BUFFER_SIZE];
    FILE *file;

    if ((file = fopen(file_name, "r")) == NULL) {
        printf("my-cat: cannot open file\n");
        exit(1);
    }

    while (fgets(buffer, BUFFER_SIZE-1, file) != NULL) {
        printf("%s", buffer);
    }
    printf("\n");
    fclose(file);
}