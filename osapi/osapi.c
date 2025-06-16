/* osapi.c */
#define Library
#include "osapi.h"

private fd fds[256];

private bool isopen(fd file){
    signed int posixfd;
    struct stat _;

    if (file<2) return false; // file descriptor 0 and 1 are reserved for stdin and stdout

    posixfd = getposixfd(file);
    if(posixfd == -1)return false;
    
    if((fstat(posixfd,&_)) == -1) return false;

    return true;
}
bool load(fd file, int8 c){
    if(file > 1){
        if(!isopen(file)){
            reterr(ErrBadFD);
        }
    }

    return false;
}

bool store(fd file){
    return 0;
    
}