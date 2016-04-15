/*
Kopiowanie plików uzywajac mmap
*/
#include <sys/mman.h>

int MEMcopy(char* pathDocelowy, char* pathZrodlowy, time_t czasZrodlowy, mode_t modeZrodlowy)
{
	int iZrodlowy, iDocelowy;
	void* source;
	struct stat s;
	size_t filesize;

	/* Zrodlo */
	if (stat(pathZrodlowy, &s) != 0) {
		/* Obsluga bledu jakby co */
	}
	filesize = s.st_size;
	iZrodlowy = open(pathZrodlowy, O_RDONLY);
	iDocelowy = open(pathDocelowy, O_RDWR | O_CREAT, modeZrodlowy);
	ftruncate(iDocelowy, filesize);

	source = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, iZrodlowy, 0);
	if (source == MAP_FAILED) {
		/* Obsluzyc blad */
		perror("mmap source");
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
		perror(pathDocelowy);
	}
	char* bname;
	bname = basename(pathDocelowy);
	loggerparam("Plik skopiowany do folderu docelowego. mmap/write",bname);
	return 0;
}