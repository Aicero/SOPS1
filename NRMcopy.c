/*
Kopiowanie plików uzywajac read/write
*/

int NRMcopy(char* pathDocelowy, mode_t modeZrodlowy) {

	/* Create the file.  */
	int fd = open(pathDocelowy, O_WRONLY | O_EXCL | O_CREAT, modeZrodlowy);
	if (fd == -1) {
		/* An error occurred. Print an error message and bail.  */
		logger("Blad podczas otwierania pliku do zapisu.");
		return 1;
	}

	return 0;
}