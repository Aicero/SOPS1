#ifndef DEMON_H_
#define DEMON_H_

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
#include <libgen.h>
#include <limits.h>
#include <utime.h>

/* FUNCTION DECLARATIONS */
void signalhandler(int);
void lsfiles(const char*,const char*);
void logerr(const char*,const  int);
void logparamerr(const char*,const char*,const int);
void rmfiles(const char*, const char*);
void cmbpath(char*, const char*, const char*);
int nrmcopy(char*, char*, time_t, mode_t);
int memmcopy(char*, char*, time_t, mode_t);

/* DEMON FLAGS & GLOBAL VALUES */
char g_pathZrodlowy[PATH_MAX + 1];
char g_pathDocelowy[PATH_MAX + 1];
int g_rekurencyjne = 0;
int g_refreshTime = 300; // 300 = 5min * 60sec
int g_progPodzialu = 0;
volatile int g_flagaSignal = 0;
volatile int g_duringSynchronization = 0;
volatile int g_verbose = 0;

#include "copy.c"
#include "lsfiles.c"
#include "logger.c"
#include "signalhandler.c"
#include "rmfiles.c"
#include "cmbpath.c"
#include "fremover.c"


#endif