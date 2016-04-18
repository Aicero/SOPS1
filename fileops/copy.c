/*
* Kopiowanie plikow uzywajac read/write
*/

int nrmcopy(const char* pathDocelowy, const char* pathZrodlowy, time_t czasZrodlowy, mode_t modeZrodlowy)
{
	int iZrodlowy, iDocelowy;
	ssize_t inputBytes, outputBytes;
	char buffer[BUF_SIZE];      /* Char buffer */

	/* Tworzenie deskryptorow */
	iZrodlowy = open(pathZrodlowy, O_RDONLY);
	if (iZrodlowy == -1) { return errno; }

	/* Tworzenie deskryptorow */
	iDocelowy = open(pathDocelowy, O_WRONLY | O_TRUNC | O_CREAT, 777);
	if (iDocelowy == -1) {
		if (close(iZrodlowy) < 0) { return errno; }
		return errno;
	}

	/* Kopiowanie */
	while ((inputBytes = read(iZrodlowy, &buffer, BUF_SIZE)) > 0) {
		outputBytes = write(iDocelowy, &buffer, (ssize_t)inputBytes);
		if (outputBytes != inputBytes) {
			if (close(iZrodlowy) < 0) {	return errno; }
			if (close(iDocelowy) < 0) { return errno; }
			return 5;
		}
	}

	/* Zamykanie deskryptorow */
	if (close(iZrodlowy) < 0) { return errno; }
	if (close(iDocelowy) < 0) { return errno; }

	/* Ustawianie czasu modyfikacji */
	struct utimbuf nowy_czas;
	nowy_czas.modtime = czasZrodlowy;
	if (utime(pathDocelowy, &nowy_czas) < 0) { return errno; }
	if (chmod(pathDocelowy, modeZrodlowy) < 0) { return errno; }
	return 0;
}


/*
* Kopiowanie plikow uzywajac mmap/write
*/
int memcopy(const char* pathDocelowy, const char* pathZrodlowy, time_t czasZrodlowy, mode_t modeZrodlowy)
{
	int iZrodlowy, iDocelowy;
	void* source;
	struct stat s;
	size_t filesize;

	/* Zrodlo */
	if (stat(pathZrodlowy, &s) != 0) { return errno; }
	
	filesize = s.st_size;

	/* Tworzenie deskryptorow */
	iZrodlowy = open(pathZrodlowy, O_RDONLY);
	if (iZrodlowy == -1) { return errno; }

	/* Tworzenie deskryptorow */
	iDocelowy = open(pathDocelowy, O_WRONLY | O_TRUNC | O_CREAT, 777);
	if (iDocelowy == -1) {
		if(close(iZrodlowy) < 0) { return errno; }
		return errno;
	}

	if(ftruncate(iDocelowy, filesize) < 0) { return errno; }

	source = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, iZrodlowy, 0);
	if (source == MAP_FAILED) {
		if(close(iZrodlowy) < 0) { return errno; }
		if(close(iDocelowy) < 0) { return errno; }
		return errno;
	}

	/* Kopiowanie */
	if (write(iDocelowy, source, filesize) < 0) { return errno; }
	if (munmap(source, filesize) < 0) { return errno; }
	if (close(iZrodlowy) < 0) { return errno; }
	if (close(iDocelowy) < 0) { return errno; }
	free(source);
	/* Ustawianie czasu modyfikacji */
	struct utimbuf nowy_czas;
	nowy_czas.modtime = czasZrodlowy;
	
	if (utime(pathDocelowy, &nowy_czas) < 0) { return errno; }
	if (chmod(pathDocelowy, modeZrodlowy) < 0) { return errno; }
	return 0;
}