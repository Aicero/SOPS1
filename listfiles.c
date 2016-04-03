/*
Wypisywanie listy plik�w dostepnych w folderze podanym jako argument.
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
      (void) closedir (dp);
    }
  else
    perror ("Nie mozna otworzyc katalogu");
}
