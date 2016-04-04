int rekurencyjne = 0;
int refreshtime = 5; // 5min * 60sec = 300
int prog_podzialu = 0;

void signalhandler(int signum);
void listfiles(char *folder, char *path);
int checkFile(char *plik);
void logger(char* message);