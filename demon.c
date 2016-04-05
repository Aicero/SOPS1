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
#include <time.h>

#include "globals.c"
#include "listfiles.c"
#include "logger.c"
#include "checkFile.c"
#include "signalhandler.c"


int main(int argc, char * argv[]) {

	struct stat sb;

	int zrodlowy = (stat(argv[1], &sb) == 0 && S_ISDIR(sb.st_mode));
	int docelowy = (stat(argv[2], &sb) == 0 && S_ISDIR(sb.st_mode));

	if (!zrodlowy || !docelowy) {
		fprintf(stderr, "%s nie jest katalogiem, lub nie istnieje.\n", zrodlowy ? argv[2] : argv[1]);
		exit(EXIT_FAILURE);
	}
	
	//Przypisuje adres katalogu docelowego i źródłowego
	realpath(argv[1],pathZrodlowy);
	realpath(argv[2],pathDocelowy);
	printf("\n %s \n %s \n", pathZrodlowy, pathDocelowy);
	
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
		syslog(LOG_NOTICE, "Program uruchomiony przez uzytkownika %d", getuid());

		/* Create a new SID for the child process */
		sid = setsid();
		if (sid < 0) {
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

			listfiles(pathZrodlowy, pathDocelowy);
			logger("Demon zostal uspiony.");
			sleep(refreshtime); /* uspienie procesu */
		}
		exit(EXIT_SUCCESS);
	}
}

//Funkcja sprawdzająca nadmiar w katalogu docelowym --> 
//odpala listfiles
//sprawdza czy któryś plik z docelowego jest w źródłowym
//jeżeli nie, usuwa plik z docelowego
