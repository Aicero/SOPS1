#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <sys/stat.h>

int g_iRecurrency = 0;
int g_iRefreshTime = 5;
int g_iSize_treshold = 0;

int main(int argc, char * argv[]) {

	struct stat sb;
	
	int zrodlowy = (stat(argv[1], &sb) == 0 && S_ISDIR(sb.st_mode));
	int docelowy = (stat(argv[2], &sb) == 0 && S_ISDIR(sb.st_mode));
	
	if(!zrodlowy){
		printf("Bledny katalog zrodlowy!\n");//tutaj dodac blad
		exit(EXIT_FAILURE);
	}
	
	if(!docelowy)
	if(!docelowy)//tutaj dodac blad zwykly zamiast dodawania folderu
	{
		printf("%s nie jest katalogiem, czy utworzyć nowy katalog? y/n\n", argv[2]);
		int c_Confirm = getchar();
		switch(c_Confirm){
			case 'y':
				mkdir(argv[2], 777);
				printf("\n Dodano katalog %s\n", argv[2]);
				docelowy = (stat(argv[2], &sb) == 0 && S_ISDIR(sb.st_mode));
				break;
			case 'n':
				printf("Nie dodano katalogu docelowego.\n");
				exit(EXIT_FAILURE);
				break;
			default:
				printf("Czy utworzyc nowy katalog? y/n");
				break;
		}
	}
	
	if (zrodlowy && docelowy) {
		printf("obie sciezki prowadza do katalogow\n\n");
		int i;
		for (i = 3; i < argc; i++) {
			if (strcmp(argv[i], "-R") == 0) {
				g_iRecurrency = 1;
			}
			else if (strcmp(argv[i], "-T") == 0) {
				if(sscanf(argv[i + 1], "%i", &g_iRefreshTime) !=1){
					printf("\nPodano bledny czas spania.\n"); //
					exit(EXIT_FAILURE);
				}
			}
			else if (strcmp(argv[i], "-S") == 0) {
				if(sscanf(argv[i + 1], "%i", &g_iSize_treshold) !=1){
					printf("\nPodano bledny prog.\n");
					exit(EXIT_FAILURE);
				}
			}
		}

		printf("rek %d\n", g_iRecurrency);
		printf("reftime %d\n", g_iRefreshTime);
		printf("prog %d\n", g_iSize_treshold);
		exit(EXIT_SUCCESS); // zakonczenie programu przed forkowaniem - tylko do testów


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

		/* Create a new SID for the child process */
		sid = setsid();
		if (sid < 0) {
			/* Log the failure */
			exit(EXIT_FAILURE);
		}



		/* Change the current working directory */
		if ((chdir("/")) < 0) {
			/* Log the failure */
			exit(EXIT_FAILURE);
		}

		/* Close out the standard file descriptors */
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);

		/* Daemon-specific initialization goes here */

		/* The Big Loop */
		while (1) {
			/* Do some task here ... */

			sleep(2); /* wait 30 seconds */
		}
		exit(EXIT_SUCCESS);
	}
}
