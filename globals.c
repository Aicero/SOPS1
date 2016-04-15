void signalhandler(int signum);
void listfiles(char *folder, char *path);
int checkFile(char *plik);
//void logger(char* message);
//void loggerparam(char* message, char* sParam);
void loggererr(char* message, int err);
void loggerparamerr(char* message, char* sParam, int err);
void removefiles(char *folderZrodlowy, char *folderDocelowy);
void combinePath(char* combined, const char* katalog, const char* nazwa);

char g_pathZrodlowy[PATH_MAX + 1];
char g_pathDocelowy[PATH_MAX + 1];
int g_rekurencyjne = 0;
int g_refreshTime = 300; // 300 = 5min * 60sec
int g_progPodzialu = 0;
volatile int g_flagaSignal = 0;
volatile int g_duringSynchronization = 0;
volatile int g_verbose = 0;