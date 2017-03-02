#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "logger.h"

static FILE *logfile;

void
startlogger()
{
    char * path = malloc(strlen(getenv("HOME")) + strlen("/.dwm/log/dwm.log"));
    sprintf(path, "%s%s", getenv("HOME"), "/.dwm/log/");
    struct stat st = {0};
    if (stat(path, &st) == -1) {
        mkdir(path, 0750);
    }
    strcat(path, "dwm.log");
    logfile = fopen(path, "a");
}

void
writelog(const char* __s)
{
    
    time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    fprintf(logfile, "%s    %s", asctime (timeinfo), __s);
    
    fflush(logfile);
}

void
endlogger()
{
    fclose(logfile);
}
