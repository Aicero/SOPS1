#define _XOPEN_SOURCE 500
#include <string.h>
#include <stdint.h>
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>



static int unlink_cb(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf)
{
    int rv = remove(fpath);
    if (rv)
        perror(fpath);
    return rv;
}

int rmrf(char *path)
{
    int flags = 0;
    flags |= FTW_DEPTH;
    flags |= FTW_PHYS;

   if (nftw(path, unlink_cb, 20, flags)
            == -1) {
        perror("nftw");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]){
	return 0;
}
/* 
int rmrf(char *path)
{
    return nftw(path, unlink_cb, 64, FTW_DEPTH | FTW_PHYS);
} */