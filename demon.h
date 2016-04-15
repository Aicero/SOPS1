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

void signalhandler(int);
void listfiles(const char*,const char*);
void loggererr(char*, int);
void loggerparamerr(char*, char*, int);
void removefiles(const char*, const char*);
void combinePath(char*, const char*, const char*);
int nrmcopy(char*, char*, time_t, mode_t);
int memmcopy(char*, char*, time_t, mode_t);

#include "globals.c"
#include "copy.c"
#include "listfiles.c"
#include "logger.c"
#include "signalhandler.c"
#include "removefiles.c"
#include "combinePath.c"
#include "fremover.c"


#endif