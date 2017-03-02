#ifndef __SELF_RESTART__
#define __SELF_RESTART__

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include "selfrestart.h"
#include "logger.h"

/**
 * Magically finds the current's executable path
 *
 * I'm doing the do{}while(); trick because Linux (what I'm running) is not
 * POSIX compilant and so lstat() cannot be trusted on /proc entries
 *
 * @return char* the path of the current executable
 */
char *
get_dwm_path(){
    struct stat s;
    int r, length, rate = 42;
    char *path = NULL;

    if(lstat("/proc/self/exe", &s) == -1){
        writelog("lstat:");
        return NULL;
    }

    length = s.st_size + 1 - rate;

    do{
        length+=rate;

        free(path);
        path = malloc(sizeof(char) * length);

        if(path == NULL){
            writelog("malloc:");
            return NULL;
        }

        r = readlink("/proc/self/exe", path, length);

        if(r == -1){
            writelog("readlink:");
            return NULL;
        }
    }while(r >= length);

    path[r] = '\0';

    return path;
}

/**
 * self-restart
 *
 * Initially inspired by: Yu-Jie Lin
 * https://sites.google.com/site/yjlnotes/notes/dwm
 */
void
restart(char * dwm_path) {
    char *const argv[] = {dwm_path, "-r", NULL};

    if(argv[0] == NULL){
        writelog("cannot get dwm path\n");
        return;
    }

    writelog("Self Restarting...\n");
    execv(argv[0], argv);
}

#endif
