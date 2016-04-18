#define _XOPEN_SOURCE 700
#include "demon.h"


int main(int argc, char * argv[]) {
	/* weryfikacja, czy obie sciezki to katalogi */
	struct stat sb;
	int zrodlowy = (stat(argv[1], &sb) == 0 && S_ISDIR(sb.st_mode));
	int docelowy = (stat(argv[2], &sb) == 0 && S_ISDIR(sb.st_mode));

	if (!zrodlowy || !docelowy) {
		fprintf(stderr, "%s nie jest katalogiem, lub nie istnieje.\n", zrodlowy ? argv[2] : argv[1]);
		exit(EXIT_FAILURE);
	}

	/* Przypisanie bezposrednich adresow katalogu docelowego i zrodlowego do zmiennych globalnych*/
	realpath(argv[1], g_pathZrodlowy);
	realpath(argv[2], g_pathDocelowy);

	if (g_pathZrodlowy == NULL || g_pathDocelowy == NULL) {
		perror("Blad rozwijania sciezki.");
		exit(EXIT_FAILURE);
	}
	
	if (g_pathZrodlowy && g_pathDocelowy) {
		int c;
		opterr = 0;

		/* Dwukropek oznacza wymagana wartosc, np -t 102 -> ok -t -> nie ok */
		while ((c = getopt(argc, argv, "RrT:t:S:s:Vv")) != -1) {
			switch (c) {
			case 'V':
			case 'v':
				flags |= VERBOSE;
				break;
			case 'R':
			case 'r':
				flags |= RECURRENCY;
				break;
			case 'T':
			case 't':
				if (sscanf(optarg, "%i", &g_refreshTime) != 1) {
					fprintf(stderr, "-- Podano bledny czas spania.\n-- Uzycie: -t \"czas w sekundach\"\n");
					exit(EXIT_FAILURE);
				}
				
				if (g_refreshTime < 0) {
					fprintf(stderr, "-- Czas spania powinien miec wartosc dodatnia.\n-- Uzycie: -t \"czas w sekundach\"\n");
					exit(EXIT_FAILURE);
				}
				break;
			case 'S':
			case 's':
				if (sscanf(optarg, "%i", &g_progPodzialu) != 1) {
					fprintf(stderr, "-- Podano bledny prog.\n-- Uzycie: -s \"prog w bajtach\"\n");
					exit(EXIT_FAILURE);
				}
				
				if (g_progPodzialu < 0) {
					fprintf(stderr, "-- Prog powinien miec wartosc dodatnia.\n-- Uzycie: -s \"prog w bajtach\"\n");
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

		/* Tworzenie pid sid */
		pid_t pid, sid;

		/* Forkowanie glownego procesu */
		pid = fork();
		if (pid < 0) {
			exit(EXIT_FAILURE);
		}
		/* Jezeli dostaniemy odpowiednie pid, zamykamy glowny proces */
		if (pid > 0) {
			exit(EXIT_SUCCESS);
		}

		/* Otwieranie logow */
		setlogmask(LOG_UPTO(LOG_INFO));
		openlog("SOPS1-demon", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
		logerr("Demon zostal uruchomiony pomyslnie.", 0);

		/* Tworzenie nowego SID */
		sid = setsid();
		if (sid < 0) {
			exit(EXIT_FAILURE);
		}

		/* Zamykanie standardowych desryptorow plikow */
		close(STDIN_FILENO);
		if (!(flags & VERBOSE)) close(STDOUT_FILENO);
		if (!(flags & VERBOSE)) close(STDERR_FILENO);

		/* The Big Loop */
		while (1) {
			/* Zmiana maski na 0000 */
			umask(0);
			/* Przypisanie handlerow sygnalow */
			signal(SIGTERM, signalhandler);
			signal(SIGUSR1, sigusrhandler);

			if (!opendir(g_pathZrodlowy) || !opendir(g_pathDocelowy)) {
				logerr(
					!opendir(g_pathZrodlowy) ? 
					"Otwarcie folderu zrodlowego nie powiodlo sie. Demon umarl." :
					"Otwarcie folderu docelowego nie powiodlo sie. Demon umarl." 
					, errno);
				exit(EXIT_FAILURE);
			}

			if (flags & SYNCHRONIZATION) {
				/* Wywolywane jezeli demon jest w trakcie synchronizacji plikow */
				continue;
			}

			if (flags & FLAG_KILL) {
				/* Wywolywane jezeli demon nie jest w trakcie synchronizacji plikow */
				closelog();
				logerr("Zakonczono dzialanie demona. [kill]", 0);
				exit(0);
			}
			
			if (!(flags & FLAG_SIGNAL)) {
				logerr("Demon wybudzony automatycznie.", 0);
				/* Ustawienie flagi SYNCHRONIZATION */
				flags |= SYNCHRONIZATION; 
				/* Rozpoczecie dzialania na folderach */
				lsfiles(g_pathZrodlowy, g_pathDocelowy);
				rmfiles(g_pathZrodlowy, g_pathDocelowy);
				/* Wylaczenie flagi SYNCHRONIZATION */
				flags &= ~SYNCHRONIZATION;
			} 
			else {
				logerr("Demon wybudzony przez SIGUSR1.", 0);
				/* Ustawienie flagi SYNCHRONIZATION */
				flags |= SYNCHRONIZATION;
				/* Rozpoczecie dzialania na folderach */
				lsfiles(g_pathZrodlowy, g_pathDocelowy);
				rmfiles(g_pathZrodlowy, g_pathDocelowy);
				/* Wylaczenie flagi SYNCHRONIZATION */
				flags &= ~SYNCHRONIZATION;
				/* Wylaczenie flagi FLAG_SIGNAL */				
				flags &= ~FLAG_SIGNAL;
			}
			
			logerr("Demon zostal uspiony.", 0);
			/* Uspienie na okreslony czas */
			sleep(g_refreshTime);
		}
		exit(EXIT_SUCCESS);
	}
}
