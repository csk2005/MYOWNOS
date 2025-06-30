#include <stdio.h>
#include "osapi.h"
#include "disk.h"
#include "fs.h"

void usage(char*);
void usage_format(char*);
int main(int,char**);
void cmd_format(char*, char*);

void cmd_format(char *arg1,char *arg2){
    int8 drive;
    int8 *drivestr;
    bool bootable;
    bool bforce;
    char force;
    filesystem *fs;
    disk *dd;

    if(!arg1) usage_format("diskutil");
    else if(!arg2){
        bootable = false;
        drivestr = $1 arg1;
    }
    else{
        if((*arg1=='-') && (arg1[1] == 's')){
            bootable = true;
            drivestr = $1 arg2;
        }
        else{
            usage_format("diskutil");
        }
    }

    switch(*drivestr){
        case 'c':
        case 'C':
            drive = 1;
            break;
        case 'd':
        case 'D':
            drive = 2;
            break;
        default:
            usage_format("diskutil");
            break;
    }

    if(bootable){
        fprintf(stderr, "Bootable currently not supported\n");
        exit(-1);
    }

    printf("This will format and ERASE your disk %s\n Force? (y/n): ", drivestr);
    fflush(stdout);

    scanf("%c", &force);
    bforce = ((force == 'y' || force == 'Y') ? true : false);

    printf("Formatting disk %c:...\n", *drivestr);

    dinit();
    dd= dattach(drive);
    if(!dd) {
        fprintf(stderr, "Bad disk\n");
        exit(-1);
    }

    fs = fsformat(dd, (bootsector *)0, bforce);

    if(fs);
    else fprintf(stderr, "Formatting Failed\n");
}

void usage(char *arg) {
    fprintf(stderr, "Usage: %s <command> [arguments]\nAvailable commands: format\n", arg);
    exit(-1);
}

void usage_format(char *arg) {
    fprintf(stderr, "Usage: %s format [-s] drive\n Example: %s format c:\n",arg,arg);
    exit(-1);
}

int main(int argc, char *argv[]){
    char *cmd, *arg1, *arg2;

    if(argc <2) usage(*argv);
    else if(argc==2) arg1 = arg2 = 0;
    else if(argc==3){
        arg1 = argv[2];
        arg2 = 0;
    } 
    else{
        arg1 = argv[2];
        arg2 = argv[3];
    }

    cmd  = argv[1];

    if(!strcmp(cmd,"format")) cmd_format(arg1,arg2);
    else usage(*argv);

    return 0;
}