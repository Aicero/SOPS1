/*
Kopiowanie plików uzywajac read/write
*/
#include <utime.h>
#define BUF_SIZE 8192

int nrmcopy(char* pathDocelowy, char* pathZrodlowy, time_t czasZrodlowy, mode_t modeZrodlowy)
{
	remove(pathDocelowy);
	int iZrodlowy, iDocelowy;
	ssize_t inputBytes, outputBytes;    /* Number of bytes returned by read() and write() */
	char buffer[BUF_SIZE];      /* Character buffer */

								/* Tworzenie deskryptorów */
	iZrodlowy = open(pathZrodlowy, O_RDONLY);
	if (iZrodlowy == -1)
	{
		//perror(pathZrodlowy);
		//fprintf(stderr, "%s\n", pathDocelowy);
		return -1;
	}

	/* Tworzenie deskryptorów */
	iDocelowy = open(pathDocelowy, O_WRONLY | O_TRUNC | O_CREAT, modeZrodlowy);
	if (iDocelowy == -1)
	{
		//perror(pathDocelowy);
		//fprintf(stderr, "%s\n", pathDocelowy);
		return -1;
	}

	/* Kopiowanie */
	while ((inputBytes = read(iZrodlowy, &buffer, BUF_SIZE)) > 0) {
		outputBytes = write(iDocelowy, &buffer, (ssize_t)inputBytes);
		if (outputBytes != inputBytes)
		{
			//perror("Problem przy kopiowaniu pliku");
			//fprintf(stderr, "%s\n", pathDocelowy);
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
		//perror(pathDocelowy);
		return -1;
	}
	char* bname;
	//bname = &(basename(pathDocelowy));
	//loggerparam("Plik skopiowany do folderu docelowego.", &(basename(pathDocelowy)));
	return 0;
}