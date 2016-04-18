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
	if (dp == NULL) {
		logparamerr("Blad otwierania katalogu zrodlowego!", folderZrodlowy, errno);
		return;
	}

	while (ep = readdir(dp)) {
		if (!strcmp(ep->d_name, ".") || !strcmp(ep->d_name, "..") /*|| ep->d_name[strlen(ep->d_name)-1] != '~'*/) {	
			/* Katalogi specjalne: . , .. zostana pominiete */
			continue;
		}

		/* Tworzenie sciezki bezposredniej do pliku/folderu */
		char s_ZrodlowyRPath[PATH_MAX + 1];
		char s_DocelowyRPath[PATH_MAX + 1];
		cmbpath(s_ZrodlowyRPath, folderZrodlowy, ep->d_name);
		cmbpath(s_DocelowyRPath, folderDocelowy, ep->d_name);

		if (lstat(s_ZrodlowyRPath, &_ZrodlowyFStruct) < 0) {
			logparamerr("Nieudana proba otwarcia pliku/folderu w folderze zrodlowym!", s_ZrodlowyRPath, errno);
			continue;
		}

		/* Sprawdzenie czy sciezka prowadzi do folderu, oraz czy rekurencja == True */
		if (S_ISDIR(_ZrodlowyFStruct.st_mode) && !(flags & RECURRENCY)) {
			/* Foldery przy wylaczonej rekurencji sa pomijane */
			continue;
		}
		else if (S_ISDIR(_ZrodlowyFStruct.st_mode))	{
			/* Jezeli folder nie istnieje w folderze docelowym - zostaje utworzony */
			if (lstat(s_DocelowyRPath, &_DocelowyFStruct) == -1) {
				/* Tworzenie folderu */
				logparamerr("Rozpoczeto kopiowanie folderu.", s_DocelowyRPath, 0);
				mkdir(s_DocelowyRPath, _ZrodlowyFStruct.st_mode);
			}
			/* Rekurencja */
			lsfiles(s_ZrodlowyRPath, s_DocelowyRPath);
			rmfiles(s_ZrodlowyRPath, s_DocelowyRPath);
			continue;
		}

		/* Sprawdzenie czy sciezka prowadzi do pliku */
		else if (S_ISREG(_ZrodlowyFStruct.st_mode))	{
			/* Sprawdzanie czy plik istnieje w katalogu docelowym: jezeli nie -> kopiujemy plik z folderu zrodlowego */
			if (lstat(s_DocelowyRPath, &_DocelowyFStruct) < 0) {
				if (g_progPodzialu == 0 || _ZrodlowyFStruct.st_size < (size_t)g_progPodzialu) {
					/* Proba utworzenia pliku za pomoca [read/write] */
					int nrmerr = nrmcopy(s_DocelowyRPath, s_ZrodlowyRPath, time(NULL), _ZrodlowyFStruct.st_mode);
					(nrmerr != 0) ?
						logparamerr("[read/write] Utworzenie pliku w katalogu docelowym nie powiodlo sie.", s_ZrodlowyRPath, nrmerr) :
						logparamerr("[read/write] Plik skopiowany do folderu docelowego.", s_DocelowyRPath, 0);
				}
				else {
					/* Proba utworzenia pliku za pomoca [mmap/write] */
					int memerr = memcopy(s_DocelowyRPath, s_ZrodlowyRPath, time(NULL), _ZrodlowyFStruct.st_mode);
					(memerr != 0) ?
						logparamerr("[mmap/write] Utworzenie pliku w katalogu docelowym nie powiodlo sie.", s_ZrodlowyRPath, memerr) :
						logparamerr("[mmap/write] Plik skopiowany do folderu docelowego.", s_DocelowyRPath, 0);
				}
			}
			else {	
				/* Plik istnial w folderze docelowym */
				time_t Czas1 = _ZrodlowyFStruct.st_mtime;
				time_t Czas2 = _DocelowyFStruct.st_mtime;
				
				/* Porownywanie czasow ostatniej modyfikacji: jezeli plik w folderze docelowym jest nowszy: kopiujemy plik */
				if (Czas1 > Czas2) {
					if (g_progPodzialu == 0 || _ZrodlowyFStruct.st_size < (size_t)g_progPodzialu) {
						/* Proba skopiowania pliku za pomoca [read/write] */
						int nrmerr = nrmcopy(s_DocelowyRPath, s_ZrodlowyRPath, Czas1, _ZrodlowyFStruct.st_mode);
						(nrmerr != 0) ?
							logparamerr("[read/write] Blad kopiowania pliku do katalogu docelowego.", s_ZrodlowyRPath, nrmerr) :
							logparamerr("[read/write] Plik skopiowany do folderu docelowego.", s_DocelowyRPath, 0);
					}
					else {
						/* Proba skopowania pliku za pomoca [mmap/write] */
						int memerr = memcopy(s_DocelowyRPath, s_ZrodlowyRPath, Czas1, _ZrodlowyFStruct.st_mode);
						(memerr != 0) ?
							logparamerr("[mmap/write] Blad kopiowania pliku do katalogu docelowego.", s_ZrodlowyRPath, memerr) :
							logparamerr("[mmap/write] Plik skopiowany do folderu docelowego.", s_DocelowyRPath, 0);
					}
				}
			}
			
			continue;
		}
		else {
			logparamerr(
				(S_ISCHR(_ZrodlowyFStruct.st_mode)) ? "Natrafiono na \"character device\". Ignorowanie pliku." :
				(S_ISBLK(_ZrodlowyFStruct.st_mode)) ? "Natrafiono na \"block device\". Ignorowanie pliku." :
				(S_ISFIFO(_ZrodlowyFStruct.st_mode)) ? "Natrafiono na \"FIFO\". Ignorowanie pliku." :
				(S_ISLNK(_ZrodlowyFStruct.st_mode)) ? "Natrafiono na \"symbolic link\". Ignorowanie pliku." :
				(S_ISSOCK(_ZrodlowyFStruct.st_mode)) ? "Natrafiono na \"socket\". Ignorowanie pliku." :				
				"NIEZNANY RODZAJ PLIKU", s_ZrodlowyRPath, 0);
		}
		free(ep);
	}
	(void)closedir(dp);
}
