/*
Wypisywanie listy plik�w dostepnych w folderze podanym jako argument.
*/
#include <time.h>

void listfiles(char *folderZrodlowy, char *folderDocelowy)
{
	struct stat file1;
	struct stat file2;
	
	DIR *dp;
	struct dirent *ep;
	
	dp = opendir(folderZrodlowy);
	if (dp != NULL)
	{
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
					// jeżeli rekurencja: 
					//czy katalog istenieje w docelowym? jeżeli nie --> tworzę katalog w docelowym
					//odpalam listfiles rekurencyjnie w katalogu (istniejącym już lub przed chwilą utworzonym)
				}
				else if (ep->d_type == DT_REG)
				{
					
					if (stat(ep->d_name, &file1) == 0) {
						}
					if ((chdir(folderDocelowy)) < 0) {
						/* Nieudana zmiana folderu na docelowy */
						fprintf(stderr,"Nieudana zmiana folderu na docelowywat\n");
						exit(EXIT_FAILURE);
					}
					
					FILE *fp = fopen(ep->d_name, "rw");
					if (!fp) {
						logger(ep->d_name);
						fprintf(stderr, "Brak pliku w drugim folderze\n");
						logger("brak pliku w drugim folderze\n");
					}
					else
					{
						logger(ep->d_name);
						fprintf(stderr, "Plik jest w drugim folderze\n");
						logger("plik jest w drugim folderze\n");
						
						time_t Czas1;
						time_t Czas2;
						
						if (stat(ep->d_name, &file2) == 0) {
							
						}
						Czas1 = file1.st_mtime;
						Czas2 = file2.st_mtime;
						fprintf(stderr, "%s",ctime(&Czas1)); // zwraca nieprawidlowy czas za pierwszym razem
						fprintf(stderr, "%s",ctime(&Czas2));
					}
					//logger(ep->d_name);

					if ((chdir(folderZrodlowy)) < 0) {
						/* Nieudana zmiana folderu na zrodlowy */
						fprintf(stderr,"Nieudana zmiana folderu na zrodlowy\n");
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
