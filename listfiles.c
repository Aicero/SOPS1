/*
Wypisywanie listy plików dostepnych w folderze podanym jako argument.
*/
#include <time.h>
#include "NRMcopy.c"

void listfiles(char *folderZrodlowy, char *folderDocelowy)
{
	struct stat file1;
	struct stat file2;

	DIR *dp;
	struct dirent *ep;

	dp = opendir(folderZrodlowy);
	if (dp != NULL)
	{
		fprintf(stderr, "\n");
		while (ep = readdir(dp))
			if (!strcmp(ep->d_name, ".") || !strcmp(ep->d_name, ".."))
			{
				continue;
				//Katalogi specjalne, nie powinno nic robic.
			}
			else
			{
				if (ep->d_type == DT_DIR)
				{
					//Katalog 
					// jeÅ¼eli rekurencja: 
					//czy katalog istenieje w docelowym? jeÅ¼eli nie --> tworzÄ™ katalog w docelowym
					//odpalam listfiles rekurencyjnie w katalogu (istniejÄ…cym juÅ¼ lub przed chwilÄ… utworzonym)
				}
				else if (ep->d_type == DT_REG)
				{
					if (stat(ep->d_name, &file1) == 0) {
						// wpisujemy jakiœ error?
					}
					if ((chdir(folderDocelowy)) < 0) {
						/* Nieudana zmiana folderu na docelowy */
						fprintf(stderr, "Nieudana zmiana folderu na docelowywat\n");
						exit(EXIT_FAILURE);
					}

					FILE *fp = fopen(ep->d_name, "rw");
					if (!fp) {
						mode_t mode;
						//logger(ep->d_name);
						fprintf(stderr, "Brak pliku w drugim folderze\n");
						//logger("brak pliku w drugim folderze\n");
						fprintf(stderr, "TWORZYMY NOWY PLIK CYKA BLYAT!!\n\n");
						fprintf(stderr, "%s", ep->d_name);
						mode = file1.st_mode; // za pierwszym razem z³a wartoœæ 
						NRMcopy(ep->d_name, mode);
					}
					else
					{
						//logger(ep->d_name);
						fprintf(stderr, "Plik jest w drugim folderze\n");
						//logger("plik jest w drugim folderze\n");

						time_t Czas1;
						time_t Czas2;


						if (stat(ep->d_name, &file2) == 0) {
							// wpisujemy jakiœ error?
						}
						Czas1 = file1.st_mtime;
						Czas2 = file2.st_mtime;
						fprintf(stderr, "%s", ctime(&Czas1)); // zwraca nieprawidlowy czas za pierwszym razem
						fprintf(stderr, "%s", ctime(&Czas2));
						if (Czas1 > Czas2) {

							mode_t mode;
							fprintf(stderr, "czas1>czas2\n");
							fprintf(stderr, "KOPIUJEMY CYKA BLYAT!!\n\n");
							mode = file1.st_mode; // za pierwszym razem z³a wartoœæ 
							NRMcopy(ep->d_name, mode);
						}
					}
					//logger(ep->d_name);

					if ((chdir(folderZrodlowy)) < 0) {
						/* Nieudana zmiana folderu na zrodlowy */
						fprintf(stderr, "Nieudana zmiana folderu na zrodlowy\n");
						exit(EXIT_FAILURE);
					}
				}
				else
				{
					logger("Natrafiono na inny typ pliku");
				}
			}
		(void)closedir(dp);
	}
	else
	{
		perror("Nie mozna otworzyc katalogu");
	}
}
