#include <ftw.h>

int unlinkthis(char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf)
{
    int rv = remove(fpath);
	loggerparam("Usuwanie: ", fpath);
    if (rv)
        perror(fpath);
    return rv;
}

int rmrf(char *path)
{
    int flags = 0;
    flags |= 8;
    flags |= 2;

   if (nftw(path, unlinkthis, 20, flags)
            == -1) {
        perror("nftw");
    }
    return 0;
}