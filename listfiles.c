/*
Wypisywanie listy plik�w dostepnych w folderze podanym jako argument.
*/

void listfiles(char *folder, char *path)
{
	DIR *dp;
	struct dirent *ep;

	dp = opendir(folder);
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
					struct stat file1;
					if (stat(ep->d_name, &file1) == 0) {
					}
					if ((chdir(path)) < 0) {
						/* Log the failure */
						exit(EXIT_FAILURE);
					}
					FILE *fp = fopen(ep->d_name, "rw");
					if (!fp) {
						logger(ep->d_name);
						logger("brak pliku w drugim folderze\n");
					}
					else
					{
						logger(ep->d_name);
						logger("plik jest w drugim folderze\n");

						struct stat file2;
						if (stat(ep->d_name, &file2) == 0) {
							printf("%i\n",ctime(file1.st_mtime));
							printf("%i\n",ctime(file2.st_mtime));
						}
					}
					//logger(ep->d_name);
					chdir("..");

					if ((chdir(folder)) < 0) {
						exit(EXIT_FAILURE);
					}

					chdir("..");
				}
				else
				{
					logger("Natrafiono na inny plik");
				}
			}
		(void)closedir(dp);
	}
	else
	{
		perror("Nie mozna otworzyc katalogu");
	}
}
