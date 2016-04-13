#include <limits.h>
#include <time.h>
#include "NRMcopy.c"
#include "combinePath.c"
#include "MEMcopy.c"


char g_pathZrodlowy[PATH_MAX + 1];
char g_pathDocelowy[PATH_MAX + 1];
int g_rekurencyjne = 0;
int g_refreshTime = 5; // 5min * 60sec = 300
int g_progPodzialu = 0;
volatile int g_flagaSignal = 0;
volatile int g_duringSynchronization = 0;
volatile int g_verbose = 0;

void signalhandler(int signum);
void listfiles(char *folder, char *path);
int checkFile(char *plik);
void logger(char* message);
void loggerparam(char* message, char* sParam);
void removefiles(char *folderZrodlowy, char *folderDocelowy);