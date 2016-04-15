/*
Kopiowanie plikow uzywajac read/write
*/
#include <utime.h>
#define BUF_SIZE 8192

int nrmcopy(char* pathDocelowy, char* pathZrodlowy, time_t czasZrodlowy, mode_t modeZrodlowy)
{
	remove(pathDocelowy);
	int iZrodlowy, iDocelowy;
	ssize_t inputBytes, outputBytes;    /* Number of bytes returned by read() and write() */
	char buffer[BUF_SIZE];      /* Character buffer */

	/* Tworzenie deskryptorow */
	iZrodlowy = open(pathZrodlowy, O_RDONLY);
	if (iZrodlowy == -1)
	{
		return -1;
	}

	/* Tworzenie deskryptorow */
	iDocelowy = open(pathDocelowy, O_WRONLY | O_TRUNC | O_CREAT, modeZrodlowy);
	if (iDocelowy == -1)
	{
		return -1;
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
		return -1;
	}
	char* bname;
	bname = basename(pathDocelowy);
	loggerparam("[read/write] Plik skopiowany do folderu docelowego.", bname);
	return 0;
}