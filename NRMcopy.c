/*
Kopiowanie plików uzywajac read/write
*/
#include <utime.h>

int NRMcopy(char* pathDocelowy,time_t czasZrodlowy, mode_t modeZrodlowy) {
	int fd = open(pathDocelowy, O_WRONLY | O_EXCL | O_CREAT, modeZrodlowy);
	if (fd == -1) {
		/* To sie wywoluje np jezeli plik juz istnieje  */
		
		/* Ustawianie czasu modyfikacji pliku docelowego na czas modyfikacji pliku zrodlowego */
		struct utimbuf nowy_czas;
		nowy_czas.modtime = czasZrodlowy;
		if (utime(pathDocelowy, &nowy_czas) < 0) {
			perror(pathDocelowy);
		}
	}
	return 0;
}