/*
* Wypisywanie listy plikow dostepnych w folderze podanym jako argument.
*/

void listfiles(const char *folderZrodlowy,const char *folderDocelowy)
{
	struct stat zrodlowyFStruct;
	struct stat docelowyFStruct;

	DIR *dp;
	struct dirent *ep;

	dp = opendir(folderZrodlowy);
	if (dp == NULL)
	{
		loggerparamerr("Blad otwierania katalogu zrodlowego!", folderZrodlowy, errno);
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

		if (stat(FileZrodlowyPath, &zrodlowyFStruct) < 0)
		{
			loggerparamerr("Nieudana proba otwarcia pliku/folderu w folderze zrodlowym!", ep->d_name, errno);
			continue;
		}
		
		/* Sprawdzenie czy sciezka prowadzi do folderu, oraz czy rekurencja == True */
		if (S_ISDIR(zrodlowyFStruct.st_mode) && !g_rekurencyjne)
		{	/* Foldery przy wyłączonej rekurencji są pomijane */
			continue;
		}
		
		if (S_ISDIR(zrodlowyFStruct.st_mode))
		{	/* Jezeli folder nie istnieje w folderze docelowym - zostaje utworzony */
			if (stat(FileDocelowyPath, &docelowyFStruct) == -1)
			{	/* Tworzenie folderu */
				loggerparamerr("Rozpoczeto kopiowanie folderu.", ep->d_name, 0);
				mkdir(FileDocelowyPath, 777);
			}
			/* Rekurencja */
			listfiles(FileZrodlowyPath, FileDocelowyPath);
			removefiles(FileZrodlowyPath, FileDocelowyPath);
			
			/* Przydzielenie praw folderu zrodlowego do docelowego */
			chmod(FileDocelowyPath, zrodlowyFStruct.st_mode);
			continue;
		}
		
		/* Sprawdzenie czy sciezka prowadzi do pliku */
		else if (S_ISREG(zrodlowyFStruct.st_mode))
		{
			time_t Czas1 = zrodlowyFStruct.st_mtime;
			/* Sprawdzanie czy plik istenieje w katalogu docelowym: jezeli nie -> kopiujemy plik z folderu zrodlowego */
			if (stat(FileDocelowyPath, &docelowyFStruct) < 0)
			{
				if (g_progPodzialu == 0 || zrodlowyFStruct.st_size < (size_t)g_progPodzialu) 
				{
					int nrmerr = nrmcopy(FileDocelowyPath, FileZrodlowyPath, time(NULL), zrodlowyFStruct.st_mode);
					if (nrmerr != 0) 
					{
						loggerparamerr("[read/write] Utworzenie pliku w katalogu docelowym nie powiodlo sie.", FileDocelowyPath, nrmerr);
					}
					else
					{
						loggerparamerr("[read/write] Plik skopiowany do folderu docelowego.", FileDocelowyPath, 0);
					}
				}
				else 
				{
					int memerr = memcopy(FileDocelowyPath, FileZrodlowyPath, time(NULL), zrodlowyFStruct.st_mode);
					if (memerr != 0) 
					{
						loggerparamerr("[mmap/write] Utworzenie pliku w katalogu docelowym nie powiodlo sie.", FileDocelowyPath, memerr);
					}
					else
					{
						loggerparamerr("[mmap/write] Plik skopiowany do folderu docelowego.", FileDocelowyPath, 0);
					}
				}
			}
			else
			{	/* Plik istnial w folderze docelowym */
				time_t Czas2 = docelowyFStruct.st_mtime;
				/* Porownywanie czasow ostatniej modyfikacji: jezeli plik w folderze docelowym jest nowszy: kopiujemy plik */
				if (Czas1 > Czas2)
				{
					mode_t mode = zrodlowyFStruct.st_mode;

					if (g_progPodzialu == 0 || zrodlowyFStruct.st_size < (size_t)g_progPodzialu) 
					{
						int nrmerr = nrmcopy(FileDocelowyPath, FileZrodlowyPath, Czas1, mode);
						if (nrmerr != 0) 
						{
							loggerparamerr("[read/write] Blad kopiowania pliku do katalogu docelowego.", FileDocelowyPath, nrmerr);
						}
						else
						{
							loggerparamerr("[read/write] Plik skopiowany do folderu docelowego.", FileDocelowyPath, 0);
						}
					}
					else {
						int memerr = memcopy(FileDocelowyPath, FileZrodlowyPath, Czas1, mode);
						if (memerr != 0) 
						{
							loggerparamerr("[mmap/write] Blad kopiowania pliku do katalogu docelowego.", FileDocelowyPath, memerr);
						}
						else
						{
							loggerparamerr("[mmap/write] Plik skopiowany do folderu docelowego.", FileDocelowyPath, 0);
						}
					}
				}
			}
			continue;
		}
		else
		{
			loggererr("Natrafiono na nieobslugiwany typ pliku.", 0);
		}
	}
	(void)closedir(dp);
}