#include "types.h"
#include "stat.h"
#include "user.h"



int main(int argc, char *argv[]) {
    int num;
    if (argc > 1) {
        printf(1, "Argument given, resetting counter...\n");
        num = getreadcount(0);
        printf(1, "Reset done!\n");
    }
    else {
        num = getreadcount(1);
    }
    printf(1, "The %s command has been called %d times.\n", "read", num);
    exit();
}