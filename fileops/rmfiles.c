/*
* Wypisywanie listy plikow dostepnych w folderze podanym jako argument.
*/

void rmfiles(const char *folderZrodlowy, const char *folderDocelowy)
{
	struct stat _FileStruct;
	DIR *dp;
	struct dirent *ep;

	dp = opendir(folderDocelowy);
	if (dp == NULL) {
		logparamerr("Nie mozna otworzyc katalogu docelowego.", folderDocelowy, errno);
		return;
	}

	while (ep = readdir(dp)) {
		if (!strcmp(ep->d_name, ".") || !strcmp(ep->d_name, "..")) {
			/* Katalogi specjalne: . , .. zostana pominiete */
			continue;
		}

		/* Tworzenie sciezki bezposredniej do pliku/folderu */
		char s_ZrodlowyRPath[PATH_MAX + 1];
		char s_DocelowyRPath[PATH_MAX + 1];
		cmbpath(s_ZrodlowyRPath, folderZrodlowy, ep->d_name);
		cmbpath(s_DocelowyRPath, folderDocelowy, ep->d_name);

		if (lstat(s_DocelowyRPath, &_FileStruct) < 0) {
			logparamerr("Nieudana proba otwarcia pliku w folderze docelowym!", ep->d_name, errno);
			continue;
		}

		/* Sprawdzenie czy sciezka prowadzi do folderu, oraz czy rekurencja == True */
		if (S_ISDIR(_FileStruct.st_mode) && !(flags & RECURRENCY)) {
			/* Foldery przy wylaczonej rekurencji sa pomijane */
			continue;
		}
		else if (S_ISDIR(_FileStruct.st_mode)) {
			/* Jezeli folder nie istnieje w katalogu zrodlowym: usuwanie folderu wraz z zawartoscia */
			if (lstat(s_ZrodlowyRPath, &_FileStruct) == -1) {
				int rmverr = rmvdir(s_DocelowyRPath);
				if (rmverr != 0) {
					logparamerr("Blad usuwania elementu z folderu docelowego.", s_DocelowyRPath, rmverr);
				}
			}
			continue;
		}

		/* Sprawdzenie czy sciezka prowadzi do pliku */
		else if (S_ISREG(_FileStruct.st_mode))
		{	/* Jezeli plik nie istnieje w katalogu zrodlowym: usuwanie pliku */
			if (access(s_ZrodlowyRPath, F_OK) == -1) {
				/* Proba usuniecia pliku */
				int rmverr = remove(s_DocelowyRPath);
				if (rmverr != 0) {
					logparamerr("Blad usuwania pliku z folderu docelowego.", s_DocelowyRPath, rmverr);
				}
				else {
					logparamerr("Usunieto plik nieobecny w folderze zrodlowym.", s_DocelowyRPath, 0);
				}
				continue;
			}
		}
		else {
			logparamerr(
				(S_ISCHR(_FileStruct.st_mode)) ? "Natrafiono na \"character device\". Ignorowanie pliku." :
				(S_ISBLK(_FileStruct.st_mode)) ? "Natrafiono na \"block device\". Ignorowanie pliku." :
				(S_ISFIFO(_FileStruct.st_mode)) ? "Natrafiono na \"FIFO\". Ignorowanie pliku." :
				(S_ISLNK(_FileStruct.st_mode)) ? "Natrafiono na \"symbolic link\". Ignorowanie pliku." :
				(S_ISSOCK(_FileStruct.st_mode)) ? "Natrafiono na \"socket\". Ignorowanie pliku." :				
		}
	}
	(void)closedir(dp);
}
