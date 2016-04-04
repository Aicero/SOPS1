/*
Wypisywanie listy plików dostepnych w folderze podanym jako argument.
*/

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

void listfiles(char *folder, char *path)
{
	DIR *dp;
	struct dirent *ep;
	char *test;

	dp = opendir(folder);
	if (dp != NULL)
	{
		while (ep = readdir(dp))
			if (!strcmp(ep->d_name, ".") || !strcmp(ep->d_name, ".."))
			{
				//Katalogi specjalne, nie powinno nic robic. 
			}
			else
			{
				if (ep->d_type == DT_DIR)
				{
					//Katalog
				}
				else if (ep->d_type == DT_REG)
				{
					//Plik
					test = path;
					strcat(test, ep->d_name);
					logger(test);
				}
			}
		(void)closedir(dp);
	}
	else
		perror("Nie mozna otworzyc katalogu");
}