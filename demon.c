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
#include <libgen.h>
#include "globals.c"
#include "listfiles.c"
#include "logger.c"
#include "signalhandler.c"
#include "removefiles.c"
#include "nrmcopy.c"
#include "combinePath.c"
#include "MEMcopy.c"


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
	realpath(argv[1], g_pathZrodlowy);
	realpath(argv[2], g_pathDocelowy);

	if (g_pathZrodlowy == NULL || g_pathDocelowy == NULL) {
		fprintf(stderr, "Blad rozwijania sciezki.");
		exit(EXIT_FAILURE);
	}

	if (g_pathZrodlowy && g_pathDocelowy) {
		int c;
		opterr = 0;

		/* drukropek oznacza wymagana wartosc jezeli uzyje sie opcji np -t 102 -> ok -t -> nie ok */
		while ((c = getopt(argc, argv, "RrT:t:S:s:Vv")) != -1) {
			switch (c)
			{
			case 'R':
			case 'r':
				g_rekurencyjne = 1;
				break;
			case 'T':
			case 't':
				if (sscanf(optarg, "%i", &g_refreshTime) != 1) {
					fprintf(stderr, "--Podano bledny czas spania.\nUzycie: -t \"czas w sekundach\"\n");
					exit(EXIT_FAILURE);
				}
				if(g_refreshTime < 0){
					fprintf(stderr, "--Czas spania powinien miec wartosc dodatnia.\nUzycie: -t \"czas w sekundach\"\n");
					exit(EXIT_FAILURE);
				}
				break;
			case 'S':
			case 's':
				if (sscanf(optarg, "%i", &g_progPodzialu) != 1) {
					fprintf(stderr, "--Podano bledny prog.\nUzycie: -s \"prog w bajtach\"\n");
					exit(EXIT_FAILURE);
				}
				if(g_progPodzialu < 0){
					fprintf(stderr, "--Prog powinien miec wartosc dodatnia.\nUzycie: -s \"prog w bajtach\"\n");
					exit(EXIT_FAILURE);
				}
				break;
			case 'V':
			case 'v':
				g_verbose = 1;
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
		if (!g_verbose) close(STDOUT_FILENO);
		if (!g_verbose) close(STDERR_FILENO);

		/* The Big Loop */
		while (1) {
			if(!opendir(g_pathZrodlowy) || !opendir(g_pathDocelowy))
			{
				logger("Jeden z folderow zostal usuniety!");
				exit(EXIT_FAILURE);
			}
			if (g_flagaSignal == 0) {
				logger("Demon wybudzony automatycznie.\n");
				listfiles(g_pathZrodlowy, g_pathDocelowy);
				removefiles(g_pathZrodlowy, g_pathDocelowy);
			}
			else {
				logger("Demon wybudzony przez SIGUSR1.\n");
				listfiles(g_pathZrodlowy, g_pathDocelowy);
				removefiles(g_pathZrodlowy, g_pathDocelowy);
				g_flagaSignal = 0;
			}
			logger("Demon zostal uspiony.\n");
			sleep(g_refreshTime);
		}
		exit(EXIT_SUCCESS);
	}
}
