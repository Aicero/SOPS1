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
		printf("\nObie sciezki prowadza do katalogow\n\n");
 		
 		int index;
 		int c; /* getopt do operowania argumentami */
 		opterr = 0;
 		
 		while ((c = getopt (argc, argv, "RrT:t:S:s:")) != -1) /* drukropek oznacza wymagana wartosc jeżeli użyje się opcji np -t 102 -> ok -t -> nie ok */
 		{
 			switch (c)
 			{
 				case 'R':
 				case 'r':
 					rekurencyjne = 1;
 					break;
 				case 'T':
 				case 't':
 					if (sscanf(optarg, "%i", &refreshtime) != 1) {
 						fprintf(stderr,"--Podano bledny czas spania.\nUzycie: -t \"czas w sekundach\"\n");
 						exit(EXIT_FAILURE);
 					}
 					break;
 				case 'S':
 				case 's':
 					if (sscanf(optarg, "%i", &prog_podzialu) != 1) {
 						fprintf(stderr,"--Podano bledny prog.\nUzycie: -s \"prog w bajtach\"\n");
 						exit(EXIT_FAILURE);
 					}
 					break;
 				case '?':
 					if (optopt == 'c')
 					{	
 						fprintf (stderr, "  Opcja -%c wymaga podania wartosci.\n", optopt); //tu cos nie chodzi
 					}
 					else if (isprint (optopt))
 					{
 						fprintf (stderr, "  Opcja -%c jest nieznana.\n", optopt);
 					}
 					else
 					{
 						fprintf (stderr, "  Unknown option character `\\x%x'.\n",	optopt);
 					}
  					exit(EXIT_FAILURE);
					break;
 				default:
 					abort ();
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
