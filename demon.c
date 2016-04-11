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
#include "removefiles.c"


int main(int argc, char * argv[]) {
	/* weryfikacja, czy obie sciezki to katalogi */
	struct stat sb;
	int zrodlowy = (stat(argv[1], &sb) == 0 && S_ISDIR(sb.st_mode));
	int docelowy = (stat(argv[2], &sb) == 0 && S_ISDIR(sb.st_mode));

	if (!zrodlowy || !docelowy) {
		fprintf(stderr, "%s nie jest katalogiem, lub nie istnieje.\n", zrodlowy ? argv[2] : argv[1]);
		exit(EXIT_FAILURE);
	}

	/* przypisanie adresow katalogu docelowego i zrodlowego */
	realpath(argv[1], pathZrodlowy);
	realpath(argv[2], pathDocelowy);
	//printf("\n %s \n %s \n", pathZrodlowy, pathDocelowy);

	if (pathZrodlowy == NULL || pathDocelowy == NULL) {
		fprintf(stderr, "Blad rozwijania sciezki.");
	}

	if (zrodlowy && docelowy) {
		int c;
		opterr = 0;

		/* drukropek oznacza wymagana wartosc jezeli uzyje sie opcji np -t 102 -> ok -t -> nie ok */
		while ((c = getopt(argc, argv, "RrT:t:S:s:")) != -1) {
			switch (c)
			{
			case 'R':
			case 'r':
				rekurencyjne = 1;
				break;
			case 'T':
			case 't':
				if (sscanf(optarg, "%i", &refreshtime) != 1) {
					fprintf(stderr, "--Podano bledny czas spania.\nUzycie: -t \"czas w sekundach\"\n");
					exit(EXIT_FAILURE);
				}
				break;
			case 'S':
			case 's':
				if (sscanf(optarg, "%i", &prog_podzialu) != 1) {
					fprintf(stderr, "--Podano bledny prog.\nUzycie: -s \"prog w bajtach\"\n");
					exit(EXIT_FAILURE);
				}
				break;
			case '?':
				if (optopt == 'T' || optopt == 't' || optopt == 'S' || optopt == 's') {
					fprintf(stderr, "  Opcja -%c wymaga podania wartosci.\n", optopt);
				}
				else if (isprint(optopt)) {
					fprintf(stderr, "  Opcja -%c jest nieznana.\n", optopt);
				}
				else {
					fprintf(stderr, "  Nieznany znak opcji `\\x%x'.\n", optopt);
				}
				exit(EXIT_FAILURE);
				break;
			default:
				abort();
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
		/* If we got a good PID, then we can exit the parent process. */
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

		/* Close out the standard file descriptors */
		close(STDIN_FILENO);
		//close(STDOUT_FILENO);
		//close(STDERR_FILENO);

		/* The Big Loop */
		while (1) {
			if (flagaSignal == 0) {
				logger("Demon wybudzony automatycznie.");
				listfiles(pathZrodlowy, pathDocelowy);
				removefiles(pathZrodlowy, pathDocelowy);
			}
			else {
				logger("Demon wybudzony przez SIGUSR1.");
				listfiles(pathZrodlowy, pathDocelowy);
				removefiles(pathZrodlowy, pathDocelowy);
				flagaSignal = 0;
			}
			logger("Demon zostal uspiony.");
			sleep(refreshtime);
		}
		exit(EXIT_SUCCESS);
	}
}
