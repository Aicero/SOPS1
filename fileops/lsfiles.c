/*
* Wypisywanie listy plikow dostepnych w folderze podanym jako argument.
*/

void lsfiles(const char *folderZrodlowy, const char *folderDocelowy)
{
	struct stat _ZrodlowyFStruct;
	struct stat _DocelowyFStruct;

	DIR *dp;
	struct dirent *ep;

	dp = opendir(folderZrodlowy);
	if (dp == NULL)
	{
		logparamerr("Blad otwierania katalogu zrodlowego!", folderZrodlowy, errno);
		return;
	}

	while (ep = readdir(dp)) {
		if (!strcmp(ep->d_name, ".") || !strcmp(ep->d_name, ".."))
		{	/* Katalogi specjalne: . , .. zostana pominiete */
			continue;
		}

		/* Tworzenie sciezki bezposredniej do pliku/folderu */
		char s_ZrodlowyRPath[PATH_MAX + 1];
		char s_DocelowyRPath[PATH_MAX + 1];
		cmbpath(s_ZrodlowyRPath, folderZrodlowy, ep->d_name);
		cmbpath(s_DocelowyRPath, folderDocelowy, ep->d_name);

		if (stat(s_ZrodlowyRPath, &_ZrodlowyFStruct) < 0)
		{
			logparamerr("Nieudana proba otwarcia pliku/folderu w folderze zrodlowym!", s_ZrodlowyRPath, errno);
			continue;
		}

		/* Sprawdzenie czy sciezka prowadzi do folderu, oraz czy rekurencja == True */
		if (S_ISDIR(_ZrodlowyFStruct.st_mode) && !(flags & RECURRENCY))
		{	/* Foldery przy wyłączonej rekurencji są pomijane */
			continue;
		}
		else if (S_ISDIR(_ZrodlowyFStruct.st_mode))
		{	/* Jezeli folder nie istnieje w folderze docelowym - zostaje utworzony */
			if (stat(s_DocelowyRPath, &_DocelowyFStruct) == -1)
			{	/* Tworzenie folderu */
				logparamerr("Rozpoczeto kopiowanie folderu.", s_DocelowyRPath, 0);
				mkdir(s_DocelowyRPath, _ZrodlowyFStruct.st_mode);
			}
			/* Rekurencja */
			lsfiles(s_ZrodlowyRPath, s_DocelowyRPath);
			rmfiles(s_ZrodlowyRPath, s_DocelowyRPath);

			/* Przydzielenie praw folderu zrodlowego do docelowego */
			chmod(s_DocelowyRPath, _ZrodlowyFStruct.st_mode);
			continue;
		}

		/* Sprawdzenie czy sciezka prowadzi do pliku */
		else if (S_ISREG(_ZrodlowyFStruct.st_mode))
		{
			time_t Czas1 = _ZrodlowyFStruct.st_mtime;
			/* Sprawdzanie czy plik istenieje w katalogu docelowym: jezeli nie -> kopiujemy plik z folderu zrodlowego */
			if (stat(s_DocelowyRPath, &_DocelowyFStruct) < 0)
			{
				if (g_progPodzialu == 0 || _ZrodlowyFStruct.st_size < (size_t)g_progPodzialu)
				{
					int nrmerr = nrmcopy(s_DocelowyRPath, s_ZrodlowyRPath, time(NULL), _ZrodlowyFStruct.st_mode);
					if (nrmerr != 0)
					{
						logparamerr("[read/write] Utworzenie pliku w katalogu docelowym nie powiodlo sie.", s_ZrodlowyRPath, nrmerr);
					}
					else
					{
						logparamerr("[read/write] Plik skopiowany do folderu docelowego.", s_DocelowyRPath, 0);
					}
				}
				else
				{
					int memerr = memcopy(s_DocelowyRPath, s_ZrodlowyRPath, time(NULL), _ZrodlowyFStruct.st_mode);
					if (memerr != 0)
					{
						logparamerr("[mmap/write] Utworzenie pliku w katalogu docelowym nie powiodlo sie.", s_ZrodlowyRPath, memerr);
					}
					else
					{
						logparamerr("[mmap/write] Plik skopiowany do folderu docelowego.", s_DocelowyRPath, 0);
					}
				}
			}
			else
			{	/* Plik istnial w folderze docelowym */
				time_t Czas2 = _DocelowyFStruct.st_mtime;
				/* Porownywanie czasow ostatniej modyfikacji: jezeli plik w folderze docelowym jest nowszy: kopiujemy plik */
				if (Czas1 > Czas2)
				{
					mode_t mode = _ZrodlowyFStruct.st_mode;

					if (g_progPodzialu == 0 || _ZrodlowyFStruct.st_size < (size_t)g_progPodzialu)
					{
						int nrmerr = nrmcopy(s_DocelowyRPath, s_ZrodlowyRPath, Czas1, mode);
						if (nrmerr != 0)
						{
							logparamerr("[read/write] Blad kopiowania pliku do katalogu docelowego.", s_ZrodlowyRPath, nrmerr);
						}
						else
						{
							logparamerr("[read/write] Plik skopiowany do folderu docelowego.", s_DocelowyRPath, 0);
						}
					}
					else {
						int memerr = memcopy(s_DocelowyRPath, s_ZrodlowyRPath, Czas1, mode);
						if (memerr != 0)
						{
							logparamerr("[mmap/write] Blad kopiowania pliku do katalogu docelowego.", s_ZrodlowyRPath, memerr);
						}
						else
						{
							logparamerr("[mmap/write] Plik skopiowany do folderu docelowego.", s_DocelowyRPath, 0);
						}
					}
				}
			}
			continue;
		}
		else
		{
			logerr("Natrafiono na nieobslugiwany typ pliku.", 0);
		}
	}
	(void)closedir(dp);
}
