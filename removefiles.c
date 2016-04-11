/*
Wypisywanie listy plików dostepnych w folderze podanym jako argument.
*/

void removefiles(char *folderZrodlowy, char *folderDocelowy)
{
	DIR *dp;
	struct dirent *ep;

	dp = opendir(folderDocelowy);
	if (dp != NULL)
	{
		fprintf(stderr, "\n");
		while (ep = readdir(dp))
			if (!strcmp(ep->d_name, ".") || !strcmp(ep->d_name, "..") /*|| ep->d_name[strlen(ep->d_name)-1] != '~'*/)
			{
				continue;
				//Katalogi specjalne, nie powinno nic robic.
			}
			else
			{
				if (ep->d_type == DT_DIR)
				{
				}
				else if (ep->d_type == DT_REG)
				{
					if ((chdir(folderDocelowy)) < 0) 
					{
						fprintf(stderr, "Nieudana zmiana folderu na docelowy\n");
						continue;
					}

					char sciezkaPlikuDocelowego[PATH_MAX +1];
					realpath(ep->d_name, sciezkaPlikuDocelowego);
					
					if ((chdir(folderZrodlowy)) < 0) 
					{
						fprintf(stderr, "Nieudana zmiana folderu na docelowy\n");
						continue;
					}
					
					if ( access(ep->d_name, F_OK ) == -1 ) 
					{
						fprintf(stderr, "Plik nie istnieje w folderze zrodlowym. Usuwamy plik %s\n", ep->d_name);
						remove(sciezkaPlikuDocelowego);
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
		perror("Nie mozna otworzyc katalogu");
	}
}
