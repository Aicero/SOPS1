#ifndef DEMON_H_
#define DEMON_H_

#include <ftw.h> /* ftw includuje jednocześnie sys/types.h sys/stat.h */
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
#include <limits.h>
#include <utime.h>

/* FUNCTION DECLARATIONS */
void signalhandler(int);
void sigusrhandler(int);
void logerr(const char*, const  int);
void logparamerr(const char*, const char*, const int);
void cmbpath(char*, const char*, const char*);

volatile int flags;

enum {
	FLAG_SIGNAL = 1 << 0,
#define FLAG_SIGNAL FLAG_SIGNAL
	SYNCHRONIZATION = 1 << 1,
#define SYNCHRONIZATION SYNCHRONIZATION
	VERBOSE = 1 << 2,
#define VERBOSE VERBOSE
	RECURRENCY = 1 << 3
#define RECURRENCY RECURRENCY
};

/* DEMON FLAGS & GLOBAL VALUES */
char g_pathZrodlowy[PATH_MAX + 1];
char g_pathDocelowy[PATH_MAX + 1];
int g_refreshTime = 300; // 300 = 5min * 60sec
int g_progPodzialu = 0;

#include "fileops/fileops.h"
#include "logger.c"
#include "signalhandler.c"
#include "cmbpath.c"


#endif