#include <limits.h>
#include <time.h>
#include "NRMcopy.c"
#include "combinePath.c"
char pathZrodlowy[PATH_MAX +1];
char pathDocelowy[PATH_MAX +1];
int rekurencyjne = 0;
int refreshtime = 5; // 5min * 60sec = 300
int prog_podzialu = 0;
volatile int flagaSignal = 0;
volatile int duringSynchronization = 0;

void signalhandler(int signum);
void listfiles(char *folder, char *path);
int checkFile(char *plik);
void logger(char* message);
void removefiles(char *folderZrodlowy, char *folderDocelowy);