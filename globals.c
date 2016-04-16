char g_pathZrodlowy[PATH_MAX + 1];
char g_pathDocelowy[PATH_MAX + 1];
int g_rekurencyjne = 0;
int g_refreshTime = 300; // 300 = 5min * 60sec
int g_progPodzialu = 0;
volatile int g_flagaSignal = 0;
volatile int g_duringSynchronization = 0;
volatile int g_verbose = 0;