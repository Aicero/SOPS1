/*
* Usuwanie folderów wraz z zawartością.
*/
#include <ftw.h>

int unlinkthis(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf)
{
	char* message;
	int rv = remove(fpath);
	if (rv) {
		return rv;
	}
	loggerparamerr(
		(tflag == FTW_DP) ? "Usunieto folder nieobecny w folderze zrodlowym." :
		(tflag == FTW_DNR) ? "Tego folderu nie mozna odczytac." :
		(tflag == FTW_F) ? "Usunieto plik nieobecny w folderze zrodlowym" :
		(tflag == FTW_SL) ? "Plik jest likiem symbolicznym.":
		"????"  , fpath, 0);
	return 0;
}

int rmrf(char *path)
{
	int flags = 0;
	flags |= FTW_DEPTH;
	flags |= FTW_PHYS;

	if (nftw(path, unlinkthis, 64, flags) == -1) {
		perror("nftw");
	}
	return 0;
}
