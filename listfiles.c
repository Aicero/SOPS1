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
					//fprintf(stderr, "Nieudana proba otwarcia pliku w folderze zrodlowym %s!\n", ep->d_name);
					loggerparam("Nieudana proga otwarcia pliku w folderze zrodlowym!", ep->d_name);
					continue;
				}

				if (ep->d_type == DT_DIR)
				{
					if (g_rekurencyjne)
					{
						if (stat(FileDocelowyPath, &file1) == -1)
						{
							mkdir(FileDocelowyPath, file1.st_mode);
						}
						listfiles(FileZrodlowyPath, FileDocelowyPath);
						removefiles(FileZrodlowyPath, FileDocelowyPath);
					}
					continue;
				}
				else if (ep->d_type == DT_REG) // tu dodaæ wybór metody kopiowania
				{
					time_t Czas1 = file1.st_mtime;

					if (stat(FileDocelowyPath, &file2) < 0)
					{
						if (g_progPodzialu == 0 /*|| rozmiarpliku < g_progPodzialu*/) {
							if (nrmcopy(FileDocelowyPath, FileZrodlowyPath, time(NULL), file1.st_mode) != 0)
							{
								// logger blad tworzenia nowego pliku
								logger("Utworzenie pliku w katalogu docelowym nie powiodlo sie.");
							}
						}
						else {
							if (MEMcopy(FileDocelowyPath, FileZrodlowyPath, time(NULL), file1.st_mode) != 0) {
								// logger blad tworzenia nowego pliku
								logger("Utworzenie pliku w katalogu docelowym nie powiodlo sie.");
							}
						}
					}
					else
					{
						time_t Czas2 = file2.st_mtime;
						if (Czas1 > Czas2)
						{
							mode_t mode = file1.st_mode;

							if (g_progPodzialu == 0 /*|| rozmiarpliku < g_progPodzialu*/) {
								if (nrmcopy(FileDocelowyPath, FileZrodlowyPath, Czas1, mode) != 0) {
									// logger blad kopiowania
									logger("Blad kopiowania pliku do katalogu docelowego.");
								}
							}
							else {
								if (MEMcopy(FileDocelowyPath, FileZrodlowyPath, Czas1, mode) != 0) {
									// logger blad kopiowania
									logger("Blad kopiowania pliku do katalogu docelowego.");
								}
							}
						}
					}
					continue;
				}
				else
				{
					logger("Natrafiono na inny typ pliku.");
				}
			}
		(void)closedir(dp);
	}
	else
	{
		// logger
		logger("Blad otwierania katalogu!");
		//perror("Nie mozna otworzyc katalogu");
	}
}
