/*
Kopiowanie plikow uzywajac mmap
*/
#include <sys/mman.h>

int memcopy(char* pathDocelowy, char* pathZrodlowy, time_t czasZrodlowy, mode_t modeZrodlowy)
{
	int iZrodlowy, iDocelowy;
	void* source;
	struct stat s;
	size_t filesize;

	/* Zrodlo */
	if (stat(pathZrodlowy, &s) != 0) {
		return errno;
	}
	filesize = s.st_size;
	iZrodlowy = open(pathZrodlowy, O_RDONLY);
	iDocelowy = open(pathDocelowy, O_RDWR | O_CREAT, 777);
	ftruncate(iDocelowy, filesize);

	source = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, iZrodlowy, 0);
	if (source == MAP_FAILED) {
		return errno;
	}

	/* Kopiowanie */
	write(iDocelowy, source, filesize);
	munmap(source, filesize);
	close(iZrodlowy);
	close(iDocelowy);

	/* Ustawianie czasu modyfikacji */
	struct utimbuf nowy_czas;
	nowy_czas.modtime = czasZrodlowy;
	if (utime(pathDocelowy, &nowy_czas) < 0)
	{
		return errno;
	}

	char* bname;
	bname = basename(pathDocelowy);
	chmod(pathDocelowy, modeZrodlowy);
	loggerparamerr("Plik skopiowany do folderu docelowego. mmap/write", bname, 0);
	return 0;
}
