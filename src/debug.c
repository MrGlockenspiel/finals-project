#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void write_log(const char* string) {
    FILE *logfile;
    logfile = fopen("log.txt", "a");

    if (logfile == NULL) {
        exit(1);
    }

    time_t rawtime;
    struct tm *info;
    time(&rawtime);
    info = localtime(&rawtime);

    fprintf(logfile, "[%.2d:%.2d:%.2d]: %s\n", info->tm_hour, info->tm_min, info->tm_sec, string);
    fclose(logfile);
}

void clear_log() {
    fclose(fopen("log.txt", "w"));
}
