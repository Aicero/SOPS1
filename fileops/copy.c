/*
* Kopiowanie plikow uzywajac read/write
*/

int nrmcopy(const char* pathDocelowy,const char* pathZrodlowy, time_t czasZrodlowy, mode_t modeZrodlowy)
{
	remove(pathDocelowy);
	int iZrodlowy, iDocelowy;
	ssize_t inputBytes, outputBytes;
	char buffer[BUF_SIZE];      /* Char buffer */

	/* Tworzenie deskryptorow */
	iZrodlowy = open(pathZrodlowy, O_RDONLY);
	if (iZrodlowy == -1)
	{
		return errno;
	}

	/* Tworzenie deskryptorow */
	iDocelowy = open(pathDocelowy, O_WRONLY | O_TRUNC | O_CREAT, 777);
	if (iDocelowy == -1)
	{
		return errno;
	}

	/* Kopiowanie */
	while ((inputBytes = read(iZrodlowy, &buffer, BUF_SIZE)) > 0) {
		outputBytes = write(iDocelowy, &buffer, (ssize_t)inputBytes);
		if (outputBytes != inputBytes)
		{
			return -1;
		}
	}

	/* Zamykanie deskryptorow */
	close(iZrodlowy);
	close(iDocelowy);

	/* Ustawianie czasu modyfikacji */
	struct utimbuf nowy_czas;
	nowy_czas.modtime = czasZrodlowy;
	if (utime(pathDocelowy, &nowy_czas) < 0)
	{
		return errno;
	}

	chmod(pathDocelowy, modeZrodlowy);
	return 0;
}


/*
* Kopiowanie plikow uzywajac mmap/write
*/
int memcopy(const char* pathDocelowy,const char* pathZrodlowy, time_t czasZrodlowy, mode_t modeZrodlowy)
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
	
	/* Tworzenie deskryptorow */
	iZrodlowy = open(pathZrodlowy, O_RDONLY);
	if (iZrodlowy == -1)
	{
		return errno;
	}

	/* Tworzenie deskryptorow */
	iDocelowy = open(pathDocelowy, O_WRONLY | O_TRUNC | O_CREAT, 777);
	if (iDocelowy == -1)
	{
		return errno;
	}
	
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

	chmod(pathDocelowy, modeZrodlowy);
	return 0;
}