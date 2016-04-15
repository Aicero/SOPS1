/*
Wypisywanie listy plikow dostepnych w folderze podanym jako argument.
*/

void removefiles(const char *folderZrodlowy, const char *folderDocelowy)
{
	struct stat file1;
	DIR *dp;
	struct dirent *ep;

	dp = opendir(folderDocelowy);
	if (dp == NULL)
	{
		loggererr("Nie mozna otworzyc katalogu.", errno);
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
			if (ep->d_type == DT_DIR && g_rekurencyjne)
			{
				if (stat(FileZrodlowyPath, &file1) == -1)
				{
					int rmverr = rmrf(FileDocelowyPath);
					if (rmverr != 0) {
						loggerparamerr("Blad usuwania elementu z folderu docelowego.", FileDocelowyPath, rmverr);
					}
				}
				continue;
			}
			else if (ep->d_type == DT_REG)
			{
				if (access(FileZrodlowyPath, F_OK) == -1)
				{
					int rmverr = remove(FileDocelowyPath);
					if (rmverr != 0) {
						loggerparamerr("Blad usuwania pliku z folderu docelowego.", FileDocelowyPath, rmverr);
					}
					else {
						loggerparamerr("Usunieto element nieobecny w folderze zrodlowym.", FileDocelowyPath, 0);
					}
					continue;
				}
			}
			else
			{
				loggererr("Natrafiono na inny typ pliku podczas sprawdzania folderu DOCELOWEGO.", 0);
			}
		}
	}
	(void)closedir(dp);
}