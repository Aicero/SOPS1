#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <dirent.h>

int rekurencyjne = 0;
int refreshtime = 5; // 5min * 60sec = 300
int prog_podzialu = 0;

void logger(char* message);
void signalhandler(int signum);
<<<<<<< HEAD
void listfiles (char *folder, char *path);
int checkFile(char *plik);
=======
void listfiles (char * folder);
int checkFile(char * plik){
>>>>>>> refs/remotes/origin/master

int main(int argc, char * argv[]) {

	struct stat sb;

	int zrodlowy = (stat(argv[1], &sb) == 0 && S_ISDIR(sb.st_mode));
	int docelowy = (stat(argv[2], &sb) == 0 && S_ISDIR(sb.st_mode));

	if (!zrodlowy || !docelowy) {
		fprintf(stderr, "%s nie jest katalogiem, lub nie istnieje.\n", zrodlowy ? argv[2] : argv[1]);
		exit(EXIT_FAILURE);
	}

	if (zrodlowy && docelowy) {
		printf("obie sciezki prowadza do katalogow\n\n");
		int i;
		for (i = 3; i < argc; i++) {
			if (strcmp(argv[i], "-R") == 0) {
				rekurencyjne = 1;
			}
			else if (strcmp(argv[i], "-T") == 0) {
				if (sscanf(argv[i + 1], "%i", &refreshtime) != 1) {
					printf("\nPodano bledny czas spania.\n"); //
					exit(EXIT_FAILURE);
				}
			}
			else if (strcmp(argv[i], "-S") == 0) {
				if (sscanf(argv[i + 1], "%i", &prog_podzialu) != 1) {
					printf("\nPodano bledny prog.\n");
					exit(EXIT_FAILURE);
				}
			}
		}

		//printf("rek %d\n", rekurencyjne);
		//printf("reftime %d\n", refreshtime);
		//printf("prog %d\n", prog_podzialu);

		signal(SIGTERM, signalhandler);
		signal(SIGUSR1, signalhandler);


		/* Our process ID and Session ID */
		pid_t pid, sid;

		/* Fork off the parent process */
		pid = fork();
		if (pid < 0) {
			exit(EXIT_FAILURE);
		}
		/* If we got a good PID, then
		   we can exit the parent process. */
		if (pid > 0) {
			exit(EXIT_SUCCESS);
		}

		/* Change the file mode mask */
		umask(0);

		/* Open any logs here */
		setlogmask(LOG_UPTO(LOG_INFO));
		openlog("SOPS1-demon", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
		//syslog(LOG_NOTICE, "Program uruchomiony przez uzytkownika %d", getuid());

		//exit(EXIT_SUCCESS); // zakonczenie programu przed forkowaniem - tylko do testow

		/* Create a new SID for the child process */
		sid = setsid();
		if (sid < 0) {
			/* Log the failure */
			exit(EXIT_FAILURE);
		}

		/* Change the current working directory - przyda sie w rekurencji!!!!!! */
		//if ((chdir("/")) < 0) {
			/* Log the failure */
		//	exit(EXIT_FAILURE);
		//}

		/* Close out the standard file descriptors */
		close(STDIN_FILENO);
		//close(STDOUT_FILENO);
		//close(STDERR_FILENO);

		/* Daemon-specific initialization goes here */

		/* The Big Loop */
		while (1) {
			logger("Demon zostal wybudzony automatycznie.");
			// tutaj wlasciwe dzialanie demona

			listfiles(argv[1], argv[2]);
			logger("Demon zostal uspiony.");
			sleep(refreshtime); /* uspienie procesu */
		}

		//closelog (); // zamkniecie logow
		exit(EXIT_SUCCESS);
	}
}

void logger(char* message) {
	//syslog(LOG_INFO, "%s", message);
	fprintf(stderr, "\n%s", message);
}

void signalhandler(int signum) {
	if (signum == SIGTERM) {
		closelog(); // zamkniecie logow
		logger("Zakonczono dzialanie demona. [kill]");
		exit(signum);
	}
	else if (signum == SIGUSR1) {
		// wywolanie metody synchronizujacej katalogi
		logger("Demon wybudzony przez SIGUSR1.");
		sleep(refreshtime); /* uspienie procesu */
	}
}

<<<<<<< HEAD
void listfiles (char *folder, char *path)
=======
void listfiles (char * folder)
>>>>>>> refs/remotes/origin/master
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
			//Można dodać pomijanie katalogów i plików ukrytych (ep-d_name[0] != '.')
		}
		else
		{
			if(ep->d_type == DT_DIR)
			{
				//Katalog 
				// jeżeli rekurencja: 
				//czy katalog istenieje w docelowym? jeżeli nie --> tworzę katalog w docelowym
				//odpalam listfiles rekurencyjnie w katalogu (istniejącym już lub przed chwilą utworzonym)
			}
<<<<<<< HEAD
			else if (ep->d_type == DT_REG)
			{
				if(stat(ep->d_name, &file1) == 0) {
				}
				if ((chdir(path)) < 0) {
					/* Log the failure */
					exit(EXIT_FAILURE);
				}
				FILE *fp = fopen(ep->d_name, "rw");
				if(!fp) {
					logger(ep->d_name);
					logger("brak pliku w drugim folderze\n");
				}
				else
				{
					logger(ep->d_name);
					logger("plik jest w drugim folderze\n");

					struct stat file1;
					struct stat file2;
					if (stat(fp->, &file2) == 0 && stat(ep->d_name, &file1) == 0) {
						logger("stated");
    					}
				}
				//logger(ep->d_name);
				chdir("..");

				if ((chdir(folder)) < 0) {
					/* Log the failure */
					exit(EXIT_FAILURE);
				}
				
				chdir("..");
			}
			else
			{
				logger("Natrafiono na inny plik");
=======
			else
			{
				//if(checkFile(ep->d_name)) --> jeżeli plik istnieje w katalogu docelowym, porównujemy
				//else --> jeżeli plik nie istnieje w katalogu docelowym, kopiujemy.
>>>>>>> refs/remotes/origin/master
			}
		}
      (void) closedir (dp);
    }
  else
    perror ("Nie mozna otworzyc katalogu");
}

<<<<<<< HEAD
int checkFile(char * plik) {

	// zwraca: 0 - pliki takie same (wazna data ostatniej modyfikacji)
	// zwraca: 1 - pliki różne -> aktualizacja
	// zwraca: -1 - plik nie istnieje -> tworzenie i kopia

=======
int checkFile(char * plik){
>>>>>>> refs/remotes/origin/master
	//sprawdza czy plik istnieje w katalogu docelowym.
	//if(istnieje) { return 1; }
	//else{ return 0; }
}

//Funkcja sprawdzająca nadmiar w katalogu docelowym --> 
//odpala listfiles
//sprawdza czy któryś plik z docelowego jest w źródłowym
<<<<<<< HEAD
//jeżeli nie, usuwa plik z docelowego
=======
//jeżeli nie, usuwa plik z docelowego
>>>>>>> refs/remotes/origin/master
