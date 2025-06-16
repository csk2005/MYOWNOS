/* osapi.c */
#define Library
#include "osapi.h"

private int8 fds[256];


// fd = 0 -> error
// fd = 1 -> stdin
// fd = 2 -> stdout


private bool isopen(fd file){
    signed int posixfd;
    struct stat _;

    if (file<3) return false; // file descriptor 0 and 1 are reserved for stdin and stdout

    posixfd = getposixfd(file);
    if(!posixfd)return false;
    
    if((fstat(posixfd,&_)) == -1) return false;

    return true;
}
public bool load(fd file, int8 c){
    int8 buf[2];
    signed int n;
    signed int posixfd;

    assert_initialized();
    
    if(file > 1){
        if(!isopen(file)){
            reterr(ErrBadFD);
        }
    }

    posixfd = getposixfd(file);
    if(!posixfd) reterr(ErrBadFD);
    
    if(posixfd == 1){
        posixfd = 0;
    }
    else if(posixfd == 2){
        posixfd = 1;
    }
    

    *buf = *(buf+1) = (int8)0;
    *buf = c;

    n=read(posixfd, $c buf, 1);
    if(n != 1) reterr(ErrIO);


    return true;
}

public int8 store(fd file){
    int8 buf[2];
    signed int n;
    signed int posixfd;

    assert_initialized();
    
    if(file > 2){
        if(!isopen(file)){
            reterr(ErrBadFD);
        }
    }

    posixfd = getposixfd(file);
    if(!posixfd) reterr(ErrBadFD);
    
    if(posixfd == 1){
        posixfd = 0;
    }
    else if(posixfd == 2){
        posixfd = 1;
    }
    

    *buf = *(buf+1) = (int8)0;

    n=read(posixfd, $c buf, 1);
    if(n != 1) reterr(ErrIO);

    return (int8)*buf;
}

private void setupfds(){
    zero($1 fds, sizeof(fds));
    fds[0] = 1; // stdin
    fds[1] = 2; // stdout

    return;
}

private void zero(int8 *str, int16 size){
    int8 *p;
    int16 n;
    for(n=size,p=str;n>0;n--,p++) *p=(int8)0;

    return;
}

public void init(void){
    errnumber = (int8)0;
    setupfds();
    initialized = true;
    return;
}