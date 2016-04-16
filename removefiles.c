/*
* Wypisywanie listy plikow dostepnych w folderze podanym jako argument.
*/

void removefiles(const char *folderZrodlowy, const char *folderDocelowy)
{
	struct stat fileStruct;
	DIR *dp;
	struct dirent *ep;

	dp = opendir(folderDocelowy);
	if (dp == NULL)
	{
		loggerparamerr("Nie mozna otworzyc katalogu docelowego.", folderDocelowy, errno);
		return;
	}

	while (ep = readdir(dp)) {
		if (!strcmp(ep->d_name, ".") || !strcmp(ep->d_name, ".."))
		{	/* Katalogi specjalne: . , .. zostana pominiete */
			continue;
		}
		
		/* Tworzenie sciezki bezposredniej do pliku/folderu */
		char FileZrodlowyPath[PATH_MAX + 1];
		char FileDocelowyPath[PATH_MAX + 1];
		combinePath(FileZrodlowyPath, folderZrodlowy, ep->d_name);
		combinePath(FileDocelowyPath, folderDocelowy, ep->d_name);
		
		if (stat(FileDocelowyPath, &fileStruct) < 0)
		{
			loggerparamerr("Nieudana proba otwarcia pliku w folderze docelowym!", ep->d_name, errno);
			continue;
		}
		
		/* Sprawdzenie czy sciezka prowadzi do folderu, oraz czy rekurencja == True */
		if (S_ISDIR(fileStruct.st_mode) && !g_rekurencyjne)
		{	/* Foldery przy wyłączonej rekurencji są pomijane */
			continue;
		}
		else if (S_ISDIR(fileStruct.st_mode))
		{	/* Jezeli folder nie istnieje w katalogu zrodlowym: usuwanie folderu wraz z zawartoscia */
			if (stat(FileZrodlowyPath, &fileStruct) == -1)
			{
				int rmverr = rmrf(FileDocelowyPath);
				if (rmverr != 0)
				{
					loggerparamerr("Blad usuwania elementu z folderu docelowego.", FileDocelowyPath, rmverr);
				}
			}
			continue;
		}
		
		/* Sprawdzenie czy sciezka prowadzi do pliku */
		else if (S_ISREG(fileStruct.st_mode))
		{	/* Jezeli plik nie istnieje w katalogu zrodlowym: usuwanie pliku */
			if (access(FileZrodlowyPath, F_OK) == -1)
			{
				int rmverr = remove(FileDocelowyPath);
				if (rmverr != 0) 
				{
					loggerparamerr("Blad usuwania pliku z folderu docelowego.", FileDocelowyPath, rmverr);
				}
				else 
				{
					loggerparamerr("Usunieto plik nieobecny w folderze zrodlowym.", FileDocelowyPath, 0);
				}
				continue;
			}
		}
		else
		{
			loggererr("Natrafiono na nieobslugiwany typ pliku podczas sprawdzania folderu DOCELOWEGO.", 0);
		}
	}
	(void)closedir(dp);
}