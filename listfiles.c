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
				}
				else if (ep->d_type == DT_REG)
				{
					if ((chdir(folderZrodlowy)) < 0) 
					{
						fprintf(stderr, "Nieudana zmiana folderu na zrodlowy\n");
						continue;
					}
					
					if (stat(ep->d_name, &file1) < 0) 
					{
						fprintf(stderr, "Nieudana proba otworzenia pliku w folderze zrodlowym %s!\n", ep->d_name);
						continue;
					}
					
					time_t Czas1 = file1.st_mtime;

					if ((chdir(folderDocelowy)) < 0) 
					{
						fprintf(stderr, "Nieudana zmiana folderu na docelowy\n");
						continue;
					}
					
					if (stat(ep->d_name, &file2) < 0) {
						fprintf(stderr, "Nieudana proba otworzenia pliku w folderze Docelowym. Tworzymy plik %s\n", ep->d_name);
						mode_t mode = file1.st_mode;
						NRMcopy(ep->d_name, time(NULL), mode);
					}
					
					else
					{
						fprintf(stderr, "Plik jest w drugim folderze\n");
						
						time_t Czas2 = file2.st_mtime;
						fprintf(stderr, "%s", ctime(&Czas1));
						fprintf(stderr, "%s", ctime(&Czas2));
						if (Czas1 > Czas2) {
							mode_t mode;
							fprintf(stderr, "czas1 > czas2\n");
							mode = file1.st_mode;
							NRMcopy(ep->d_name, Czas1, mode);
						}
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
