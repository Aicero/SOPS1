#ifndef DEMON_H_
#define DEMON_H_

/* 
* obsługa nftw -> file tree walk, do przechodzenia po drzewie katalogów i wykonywania operacji na każdym znalezionym pliku
* ftw zawiera jednocześnie #include bibliotek sys/types.h sys/stat.h 
*/
#include <ftw.h>

/* Standard input/output */
#include <stdio.h>

/* Biblioteka do obslugi sygnalow */
#include <signal.h>

/* Obsluga funkcji exit() */
#include <stdlib.h>

/* Obsluga funkcji open() */
#include <fcntl.h>

/* Zawiera definicje dla numerow bledow */
#include <errno.h>

/* Obsluga getopt i powiazanych */
#include <unistd.h>

/* Obsluga logow systemowych */
#include <syslog.h> 

/* Obsluga operacji na tablicach znakow */
#include <string.h> 

/* Obsluga readdir() opendir() */
#include <dirent.h> 

/* Obsluga zmiennych time_t */
#include <time.h> 

/* Uzywane jedynie do decinicji PATH_MAX */
#include <limits.h> 

/* Konwersja typow time_t do bardziej przyjaznego formatu */
#include <utime.h> 

/* FUNCTION DECLARATIONS */

void signalhandler(int);
void sigusrhandler(int);
void logerr(const char*, const int);
void logparamerr(const char*, const char*, const int);
void cmbpath(char*, const char*, const char*);

/* Zmienna przechowujaca ustawione flagi */
volatile int flags;

/* Definicje flag */
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
volatile int g_refreshTime = 300; // 300 = 5min * 60sec
volatile int g_progPodzialu = 0;

/* Wykorzystywane pliki */
#include "fileops/fileops.h"
#include "logger.c"
#include "signalhandler.c"
#include "cmbpath.c"


#endif