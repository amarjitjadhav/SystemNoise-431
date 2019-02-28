// This program will grab all currently running processes and
// calculate their average CPU usage over their lifetime.

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>

typedef struct process { //for organizing processes
    int pid;
    char name[100];
    double uTime;
    double sTime;
    double tTime;
    double cuTime;
    double csTime;
    double ctTime;
    double startTime;
    double cpuPercent;
} process;

int calcCPU(process *, int);
int display(process *, int);
int compare(const void *, const void *);

int main() {
    char statList[1000][100];
    char statusList[1000][100];
    FILE * statPtr, statusPtr;
    DIR * dirPtr;
    struct dirent * entry;
    dirPtr = opendir("/proc");
    chdir("/proc");

    for(int i = 0; i < 1000; ++i) // Initialize statList so nothing stupid happens
        for(int j = 0; j < 10; ++j)
            statList[i][j] = '\0';

    // Grab directories
    int count = -1;
    while( (entry = readdir(dirPtr)) ) {
        if(entry->d_name[0] > '0' && entry->d_name[0] < '9') {
            sprintf(statList[++count], "/proc/%s/stat", entry->d_name);
            sprintf(statusList[++count], "/proc/%s/status", entry->d_name);
        }
    }

    process * procArray = malloc(sizeof(process) * count);
    for(int i = 0; i < count; ++i) {
        statPtr = fopen(statList[i], "r");
        statusPtr = fopen(statusList[i], "r");
        // Use backslash to split stupid fscanf line
        fscanf(statPtr,
            "%d %s %*c %*d %*d %*d %*d %*d %*u %*lu %*lu %*lu %*lu %lu %lu %ld %ld %*ld %*ld %*ld %*ld %llu",
            &procArray[i].pid, procArray[i].name,
            &procArray[i].uTime, &procArray[i].sTime,
            &procArray[i].cuTime, &procArray[i].csTime,
            &procArray[i].startTime
        );
        fclose(statusList[i], "r");
        fclose(statPtr);
    }

    if(!calcCPU(procArray, count)) {
        fprintf(stderr, "\nError at calcCPU.\n");
        exit(EXIT_FAILURE);
    }

    qsort(procArray, count, sizeof(process), compare);

    if(!display(procArray, count)) {
        fprintf(stderr, "\nError at display.\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}

int calcCPU(process * procArray, int size) {
    if(!procArray)
        return 0;

    double Hz = sysconf(_SC_CLK_TCK);
    double sysUpTime;
    FILE * statPtr = fopen("/proc/uptime", "r");
    fscanf(statPtr, "%llu", &sysUpTime);
    fclose(statPtr);

    double seconds;
    for(int i = 0; i < size; ++i) {
        procArray[i].tTime = procArray[i].uTime + procArray[i].sTime; // Calculate total time for process
        procArray[i].ctTime = procArray[i].tTime + procArray[i].cuTime + procArray[i].csTime; // Calculate total time but include children time also
        seconds = sysUpTime - (procArray[i].startTime / Hz); // Turn the total elapsed time since the process began into units of seconds
        procArray[i].cpuPercent = (double) (100 * ((procArray[i].tTime / Hz) / seconds)); // Calculate cpu usage %
    }
    return 1;
}

int display(process * procArray, int size) {
    if(!procArray)
        return 0;
    for(int i = 0; i < size; ++i) {
        printf("%d: %s\n\tPID: %d\n\tAvg CPU Time: %lf\n",
                i,
                procArray[i].name,
                procArray[i].pid,
                procArray[i].cpuPercent
        );
    }
    return 1;
}

int compare(const void * a, const void * b) {
    double cpuTimeA = (double)((process*)a)->cpuPercent;
    double cpuTimeB = (double)((process*)b)->cpuPercent;
    if(cpuTimeA > cpuTimeB)
        return 1;
    if(cpuTimeA < cpuTimeB)
        return -1;
    else
         return 0;
}










