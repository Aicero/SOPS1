#include <ftw.h>
#define FTW_DEPTH 8
#define FTW_PHYS 2

int unlinkthis(char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf)
{
	int rv = remove(fpath);
	if (rv) {
		return rv;
	}
	loggerparamerr("Usunieto element nieobecny w folderze zrodlowym.", fpath, 0);
	return 0;
}

int rmrf(char *path)
{
	int flags = 0;
	flags |= FTW_DEPTH;
	flags |= FTW_PHYS;

	if (nftw(path, unlinkthis, 20, flags) == -1) {
		perror("nftw");
	}
	return 0;
}
