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
			switch (c)
			{
			case 'R':
			case 'r':
				g_rekurencyjne = 1;
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

		/* Change the file mode mask */
		umask(0);

		/* Open any logs here */
		setlogmask(LOG_UPTO(LOG_INFO));
		openlog("SOPS1-demon", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
		logerr("Demon zostal uruchomiony pomyslnie.", 0);

		/* Create a new SID for the child process */
		sid = setsid();
		if (sid < 0) {
			exit(EXIT_FAILURE);
		}

		/* Zamykanie standardowych desryptorow plikow */
		close(STDIN_FILENO);
		if (!g_verbose) close(STDOUT_FILENO);
		if (!g_verbose) close(STDERR_FILENO);

		/* The Big Loop */
		while (1) {
			if (!opendir(g_pathZrodlowy) || !opendir(g_pathDocelowy))
			{
				logerr("Otwarcie folderu zrodlowego lub docelowego nie powiodlo sie. Demon umarl.", errno);
				exit(EXIT_FAILURE);
			}
			if (g_flagSignal == 0) {
				logerr("Demon wybudzony automatycznie.", 0);
				g_duringSynchronization = 1;
				lsfiles(g_pathZrodlowy, g_pathDocelowy);
				rmfiles(g_pathZrodlowy, g_pathDocelowy);
				g_duringSynchronization = 0;
			}
			else {
				if (g_duringSynchronization == 1) {
					g_flagSignal = 0;
					sleep(g_refreshTime);
					continue;
				}
				logerr("Demon wybudzony przez SIGUSR1.", 0);
				g_duringSynchronization = 1;
				lsfiles(g_pathZrodlowy, g_pathDocelowy);
				rmfiles(g_pathZrodlowy, g_pathDocelowy);
				g_duringSynchronization = 0;
				g_flagSignal = 0;
			}
			logerr("Demon zostal uspiony.", 0);
			sleep(g_refreshTime);
		}
		exit(EXIT_SUCCESS);
	}
}
