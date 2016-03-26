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

int main(int argc, char * argv[]) {

    struct stat sb;
    int zrodlowy = (stat(argv[1], &sb) == 0 && S_ISDIR(sb.st_mode));
    int docelowy = (stat(argv[2], &sb) == 0 && S_ISDIR(sb.st_mode));

    if(zrodlowy && docelowy) {
        printf("obie sciezki prowadza do katalogow\n\n");
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
        } else {
            printf("ktoras sciezka okazala sie nie byc katalogiem\n");
            printf("TO JEST KOD BLEDU ... KTORY TRZEBA EDYTOWAC\n\n");
            exit(EXIT_FAILURE);
        }
    }
