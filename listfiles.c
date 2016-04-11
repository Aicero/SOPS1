/*
Wypisywanie listy plików dostepnych w folderze podanym jako argument.
*/
#include <time.h>
#include "NRMcopy.c"
#include "combinePath.c"
void removefiles(char *folderZrodlowy, char *folderDocelowy);

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
			
			if (!strcmp(ep->d_name, ".") || !strcmp(ep->d_name, "..") /*|| ep->d_name[strlen(ep->d_name)-1] != '~'*/)
			{
				continue;
				//Katalogi specjalne, nie powinno nic robic.
			}
			else
			{
				char FileZrodlowyPath[PATH_MAX +1];
				char FileDocelowyPath[PATH_MAX +1];
				combinePath(FileZrodlowyPath, folderZrodlowy, ep->d_name);
				combinePath(FileDocelowyPath, folderDocelowy, ep->d_name);
				
				if (stat(FileZrodlowyPath, &file1) < 0) 
				{
					fprintf(stderr, "Nieudana proba otworzenia pliku w folderze zrodlowym %s!\n", ep->d_name);
					continue;
				}
				
				if (ep->d_type == DT_DIR && rekurencyjne) // tutaj dodac opcje z rekurencja
				{
					if (stat(FileDocelowyPath, &file1) == -1) 
					{
						mkdir(FileDocelowyPath, file1.st_mode);
					}
					listfiles(FileZrodlowyPath, FileDocelowyPath);
					removefiles(FileZrodlowyPath, FileDocelowyPath);
					continue;
				}
				else if (ep->d_type == DT_REG)
				{					
					
					
					time_t Czas1 = file1.st_mtime;

					if (stat(FileDocelowyPath, &file2) < 0) 
					{
						fprintf(stderr, "Nieudana proba otworzenia pliku w folderze Docelowym. Tworzymy plik %s\n", ep->d_name);
						NRMcopy(FileDocelowyPath, FileZrodlowyPath, time(NULL), file1.st_mode);
					}
					
					else
					{
						fprintf(stderr, "Plik jest w drugim folderze\n");
						
						time_t Czas2 = file2.st_mtime;
						fprintf(stderr, "%s", ctime(&Czas1));
						fprintf(stderr, "%s", ctime(&Czas2));
						if (Czas1 > Czas2) 
						{
							mode_t mode;
							fprintf(stderr, "czas1 > czas2\n");
							mode = file1.st_mode;
							NRMcopy(FileDocelowyPath, FileZrodlowyPath, Czas1, mode);
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
