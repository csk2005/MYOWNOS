/* command.c */
#include "command.h"

int main(int argc, char *argv[]) {
    init();
    printf("Command line arguments:\n");
    fflush(stdout);  // Force output
    int8 *p;

    printf("Command line arguments:\n");

    // int8 c;
    // c = store((int8)0);
    // printf("c = 0x%.02hhx\n",$i c);
    // printf("errornumber = 0x%.02hhx\n",$i errnumber);

    load((int8)1, 'X');
    printf("\n\n");

    p = strnum($1 "drive.", 1);
    printf("p = '%s'\n", p);

    return 0;
}