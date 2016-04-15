/*
Wypisywanie listy plikow dostepnych w folderze podanym jako argument.
*/

void listfiles(char *folderZrodlowy, char *folderDocelowy)
{
	struct stat file1;
	struct stat file2;

	DIR *dp;
	struct dirent *ep;

	dp = opendir(folderZrodlowy);
	if (dp == NULL)
	{
		loggererr("Blad otwierania katalogu!", errno);
		return;
	}

	while (ep = readdir(dp)) {

		if (!strcmp(ep->d_name, ".") || !strcmp(ep->d_name, ".."))
		{
			continue;
			//Katalogi specjalne, nie powinno nic robic.
		}
		else
		{
			if (ep->d_type == DT_LNK || ep->d_type == DT_UNKNOWN) {
				//Trzeba dodac tu inne typy
				continue;
			}

			char FileZrodlowyPath[PATH_MAX + 1];
			char FileDocelowyPath[PATH_MAX + 1];
			combinePath(FileZrodlowyPath, folderZrodlowy, ep->d_name);
			combinePath(FileDocelowyPath, folderDocelowy, ep->d_name);

			if (stat(FileZrodlowyPath, &file1) < 0)
			{
				loggerparamerr("Nieudana proba otwarcia pliku w folderze zrodlowym!", ep->d_name, errno);
				continue;
			}

			if (ep->d_type == DT_DIR && g_rekurencyjne)
			{
				if (stat(FileDocelowyPath, &file1) == -1)
				{
					mkdir(FileDocelowyPath, 777);
				}
				listfiles(FileZrodlowyPath, FileDocelowyPath);
				removefiles(FileZrodlowyPath, FileDocelowyPath);
				chmod(FileDocelowyPath, file1.st_mode);
				continue;
			}
			else if (ep->d_type == DT_REG)
			{
				time_t Czas1 = file1.st_mtime;

				if (stat(FileDocelowyPath, &file2) < 0)
				{
					if (g_progPodzialu == 0 || file1.st_size < (size_t)g_progPodzialu) {
						int nrmerr = nrmcopy(FileDocelowyPath, FileZrodlowyPath, time(NULL), file1.st_mode);
						if (nrmerr != 0) {
							loggererr("[read/write] Utworzenie pliku w katalogu docelowym nie powiodlo sie.", nrmerr);
						}
					}
					else {

						int memerr = memcopy(FileDocelowyPath, FileZrodlowyPath, time(NULL), file1.st_mode);
						if (memerr != 0) {
							loggererr("[mmap/write] Utworzenie pliku w katalogu docelowym nie powiodlo sie.", memerr);
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
							int nrmerr = nrmcopy(FileDocelowyPath, FileZrodlowyPath, Czas1, mode);
							if (nrmerr != 0) {
								loggererr("[read/write] Blad kopiowania pliku do katalogu docelowego.", nrmerr);
							}
						}
						else {
							int memerr = memcopy(FileDocelowyPath, FileZrodlowyPath, Czas1, mode);
							if (memerr != 0) {
								loggererr("[mmap/write] Blad kopiowania pliku do katalogu docelowego.", memerr);
							}
						}
					}
				}
				continue;
			}
			else
			{
				loggererr("Natrafiono na inny typ pliku.", 0);
			}
		}
	}
	(void)closedir(dp);
}