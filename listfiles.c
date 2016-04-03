/*
Wypisywanie listy plików dostepnych w folderze podanym jako argument.
*/

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

void listfiles (char * folder)
{
  DIR *dp;
  struct dirent *ep;

  dp = opendir (folder);
  if (dp != NULL)
    {
      while (ep = readdir (dp))
	    if(!strcmp(ep->d_name,".") || !strcmp(ep->d_name,".."))
		{
			//Katalogi specjalne, nie powinno nic robic. 
		}
		else
		{
			if(ep->d_type == DT_DIR)
			{
				//Katalog
			}
			else
			{
				//Plik
			}
		}
      (void) closedir (dp);
    }
  else
    perror ("Nie mozna otworzyc katalogu");
}