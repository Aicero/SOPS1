#ifndef FILEOPS_H_
#define FILEOPS_H_


#include <sys/mman.h>
#include "../demon.h"
#define BUF_SIZE 8192

void lsfiles(const char*,const char*);
void rmfiles(const char*, const char*);
void cmbpath(char*, const char*, const char*);
int nrmcopy(const char*,const char*, time_t, mode_t);
int memmcopy(const char*,const char*, time_t, mode_t);

#include "copy.c"
#include "lsfiles.c"
#include "rmfiles.c"
#include "rmvdir.c"

#endif