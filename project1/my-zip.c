#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    char *buffer = NULL, *saveBuffer = NULL;
    FILE *file;
    size_t size = 0;
    int len, count, savedSize;
    char new_char;


    if ((file = fopen(argv[1], "r")) == NULL) {
        perror("Cannot open file\n");
        exit(1);
    }
    int i;
    while ((len = getline(&buffer, &size, file)) != -1) {
        if (len > savedSize) {
            if (saveBuffer == NULL) {
                saveBuffer = (char *)malloc(sizeof(char) * savedSize);
            }
            else {
                saveBuffer = realloc(saveBuffer, savedSize);
            }
            savedSize = len;
        }
        for (i = 0; i < len; i++) {
            /* Current character in string */
            new_char = buffer[i];
            count = 1;
            /* While loop for counting how many same characters
            there are in a row */
            while (buffer[i+1] == new_char) {
                /* Increase character count by one and move to the next
                character by increasing i by one */
                i++;
                count++;
            }
            /* printf("%d%c", count, new_char); */

            fwrite(&count, sizeof(count) , 1, stdout);
            fwrite(&new_char, sizeof(new_char), 1, stdout);
        }

        
    }

    if (buffer != NULL) {
        free(buffer);
    }
    if (saveBuffer != NULL) {
        free(saveBuffer);
    }
}