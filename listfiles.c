/*
Wypisywanie listy plików dostepnych w folderze podanym jako argument.
*/

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

			if (!strcmp(ep->d_name, ".") || !strcmp(ep->d_name, "..") /*|| ep->d_name[strlen(ep->d_name)-1] != '~'*/)
			{
				continue;
				//Katalogi specjalne, nie powinno nic robic.
			}
			else
			{
				char FileZrodlowyPath[PATH_MAX + 1];
				char FileDocelowyPath[PATH_MAX + 1];
				combinePath(FileZrodlowyPath, folderZrodlowy, ep->d_name);
				combinePath(FileDocelowyPath, folderDocelowy, ep->d_name);

				if (stat(FileZrodlowyPath, &file1) < 0)
				{
					// logger
					fprintf(stderr, "Nieudana proba otwarcia pliku w folderze zrodlowym %s!\n", ep->d_name);
					continue;
				}

				if (ep->d_type == DT_DIR && g_rekurencyjne)
				{
					if (stat(FileDocelowyPath, &file1) == -1)
					{
						mkdir(FileDocelowyPath, file1.st_mode);
					}
					listfiles(FileZrodlowyPath, FileDocelowyPath);
					removefiles(FileZrodlowyPath, FileDocelowyPath);
					continue;
				}
				else if (ep->d_type == DT_REG) // tu dodaæ wybór metody kopiowania
				{
					time_t Czas1 = file1.st_mtime;

					if (stat(FileDocelowyPath, &file2) < 0)
					{
						// logger
						fprintf(stderr, "Nieudana proba otworzenia pliku w folderze Docelowym. Tworzymy plik %s\n", ep->d_name);
						if (NRMcopy(FileDocelowyPath, FileZrodlowyPath, time(NULL), file1.st_mode) != 0)
						{
							// logger blad tworzenia nowego pliku
						}

						//MEMcopy(FileDocelowyPath, FileZrodlowyPath, time(NULL), file1.st_mode);
					}
					else
					{
						time_t Czas2 = file2.st_mtime;
						if (Czas1 > Czas2)
						{
							mode_t mode = file1.st_mode;
							if (NRMcopy(FileDocelowyPath, FileZrodlowyPath, Czas1, mode) != 0) {
								// logger blad kopiowania
							}
							//MEMcopy(FileDocelowyPath, FileZrodlowyPath, Czas1, mode);
						}
					}
				}
				else
				{
					// logger
					logger("Natrafiono na inny typ pliku");
				}
			}
		(void)closedir(dp);
	}
	else
	{
		// logger
		perror("Nie mozna otworzyc katalogu");
	}
}
