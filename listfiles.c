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

			if (!strcmp(ep->d_name, ".") || !strcmp(ep->d_name, ".."))
			{
				continue;
				//Katalogi specjalne, nie powinno nic robic.
			}
			else
			{
				if (ep->d_type == DT_LNK || ep->d_type == DT_UNKNOWN){
					//Trzeba dodac tu inne typy
					continue;
				}
				char FileZrodlowyPath[PATH_MAX + 1];
				char FileDocelowyPath[PATH_MAX + 1];
				combinePath(FileZrodlowyPath, folderZrodlowy, ep->d_name);
				combinePath(FileDocelowyPath, folderDocelowy, ep->d_name);
				
				if (stat(FileZrodlowyPath, &file1) < 0)
				{
					loggerparam("Nieudana proba otwarcia pliku w folderze zrodlowym!", ep->d_name);
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
				else if (ep->d_type == DT_REG)
				{
					time_t Czas1 = file1.st_mtime;

					if (stat(FileDocelowyPath, &file2) < 0)
					{
						if (g_progPodzialu == 0 || file1.st_size < (size_t)g_progPodzialu) {
							if (nrmcopy(FileDocelowyPath, FileZrodlowyPath, time(NULL), file1.st_mode) != 0)
							{
								// logger blad tworzenia nowego pliku
								logger("[read/write] Utworzenie pliku w katalogu docelowym nie powiodlo sie.");
							}
						}
						else {
							if (memcopy(FileDocelowyPath, FileZrodlowyPath, time(NULL), file1.st_mode) != 0) {
								// logger blad tworzenia nowego pliku
								logger("[mmap/write] Utworzenie pliku w katalogu docelowym nie powiodlo sie.");
							}
						}
					}
					else
					{
						time_t Czas2 = file2.st_mtime;
						if (Czas1 > Czas2)
						{
							mode_t mode = file1.st_mode;

							if (g_progPodzialu == 0 || file1.st_size < (size_t)g_progPodzialu) {
								if (nrmcopy(FileDocelowyPath, FileZrodlowyPath, Czas1, mode) != 0) {
									// logger blad kopiowania
									logger("[read/write] Blad kopiowania pliku do katalogu docelowego.");
								}
							}
							else {
								if (memcopy(FileDocelowyPath, FileZrodlowyPath, Czas1, mode) != 0) {
									// logger blad kopiowania
									logger("[mmap/write] Blad kopiowania pliku do katalogu docelowego.");
								}
								else {
									loggerparam("[mmap/write] Plik skopiowany do folderu docelowego.", ep->d_name);
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
