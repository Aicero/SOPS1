/*
Wypisywanie listy plików dostepnych w folderze podanym jako argument.
*/

void removefiles(char *folderZrodlowy, char *folderDocelowy)
{
	struct stat file1;
	DIR *dp;
	struct dirent *ep;

	dp = opendir(folderDocelowy);
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
				if (ep->d_type == DT_LNK || ep->d_type == DT_UNKNOWN){
					//Trzeba dodac tu inne typy
					continue;
				}
				char FileZrodlowyPath[PATH_MAX + 1];
				char FileDocelowyPath[PATH_MAX + 1];
				combinePath(FileZrodlowyPath, folderZrodlowy, ep->d_name);
				combinePath(FileDocelowyPath, folderDocelowy, ep->d_name);
				if (ep->d_type == DT_DIR)
				{
					if(g_rekurencyjne)
					{
						if (stat(FileZrodlowyPath, &file1) == -1)
						{
							// logger
							fprintf(stderr, "Folder nie istnieje w folderze zrodlowym. Usuwamy folder %s\n", FileDocelowyPath);
							rmrf(FileDocelowyPath);
						}
					}
					continue;
				}
				else if (ep->d_type == DT_REG)
				{
					if (access(FileZrodlowyPath, F_OK) == -1)
					{
						// logger
						fprintf(stderr, "Plik nie istnieje w folderze zrodlowym. Usuwamy plik %s\n", FileDocelowyPath);
						remove(FileDocelowyPath);
						continue;
					}
				}
				else
				{
					logger("Natrafiono na inny typ pliku podczas sprawdzania folderu DOCELOWEGO");
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
