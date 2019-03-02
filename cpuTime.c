/*
 * Kobe Davis
 * Prof. Karavanic
 * CS 431
 * 1 March 2019
 *
 * Group Research Project: Detecting and Mitigating System Noise
*/


/* This program will grab all currently running processes and
 * calculate, for each, their average CPU usage over their lifetime.
*/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

typedef struct process {
    int pid;
    int uid;
    char name[100];
    long double uTime;
    long double sTime;
    long double tTime;
    long double cuTime;
    long double csTime;
    long double ctTime;
    long double startTime;
    long double cpuPercent;
} process;

int calcCPU(process **, int);
int displayAll(process **, int);
int display(process *);
int compareAP(const void *, const void *);
int compareTT(const void *, const void *);
int compareTTC(const void *, const void *);
int grabDirs(char ***, char ***);
int allocateProcs(process ***, char **, char **, int);

int main() {
    char ** stat = NULL;
    char ** status = NULL;
    process ** procArray = NULL;
    int numDir = grabDirs(&stat, &status);
    int numProc = allocateProcs(&procArray, stat, status, numDir);

    if(!calcCPU(procArray, numProc)) {
        fprintf(stderr, "\nError at calcCPU.\n");
        exit(EXIT_FAILURE);
    }

    qsort(procArray, numProc, sizeof(process*), compareAP);

    if(!displayAll(procArray, numProc)) {
        fprintf(stderr, "\nError at display.\n");
        exit(EXIT_FAILURE);
    }

    // need to put 3 writes and 3 qsorts right here (that includes the above qsort)
    // will have 3 data files which have data sorted by:
    //      1. avg cpu usage per lifetime
    //      2. total cpu usage
    //      3. total cpu usage including child (turns out there are processes with low self cpu time but reallly high child cpu time)
    //
    //  afterwards, loop that sleeps and samples for some given amount of time should be placed
    //  before grabDirs() and after last write.

    exit(EXIT_SUCCESS);
}

// Comparator function for built-in qsort.
// Will compare total cpu time w/ child time included, 
// (user + kernel + child user + child kernel).
int compareTTC(const void * a, const void * b) {
    process * A = *((process **)a);
    process * B = *((process **)b);
    long double ctotalA = A->ctTime;
    long double ctotalB = B->ctTime;

    if(ctotalA > ctotalB)
        return 1;
    if(ctotalA < ctotalB)
        return -1;
    else
         return 0;
}

// Comparator function for built-in qsort.
// Will compare total cpu time (user + kernel).
int compareTT(const void * a, const void * b) {
    process * A = *((process **)a);
    process * B = *((process **)b);
    long double totalA = A->tTime;
    long double totalB = B->tTime;

    if(totalA > totalB)
        return 1;
    if(totalA < totalB)
        return -1;
    else
         return 0;
}

// Comparator function for built-in qsort.
// Will compare average percent of cpu used during process lifetime.
int compareAP(const void * a, const void * b) {
    process * A = *((process **)a); // Ugh
    process * B = *((process **)b);
    long double percentA = A->cpuPercent;
    long double percentB = B->cpuPercent;

    if(percentA < percentB)
        return 1;
    if(percentA > percentB)
        return -1;
    else
         return 0;
}

// Output of grabDirs is stat, status, and numDir.
// Both stat and status will be output as dynamically allocated arrays which store the process directory pathnames.
// numDir represents the size of both stat and status (same size).
int grabDirs(char *** stat, char *** status) {
    int numDir = 0;
    int myPID = getpid();
    char statList[1000][100];       // Temp array to hold paths to ALL process stat files
    char statusList[1000][100];     // Temp array to hold paths to ALL process status files
    struct dirent * entry = NULL;
    DIR * dirPtr = opendir("/proc");
    chdir("/proc");

    while( (entry = readdir(dirPtr)) ) { // Read all directory entries in proc
        // Filter out directories non-specific to processes and also process running a.out as it causes errors
        if(myPID != atoi(entry->d_name) && entry->d_name[0] > '0' && entry->d_name[0] < '9') {
            sprintf(statList[numDir], "/proc/%s/stat", entry->d_name);
            sprintf(statusList[numDir++], "/proc/%s/status", entry->d_name);
        }
    }

    *stat = malloc(sizeof(char *) * numDir);
    *status = malloc(sizeof(char *) * numDir);
    for(int i = 0; i < numDir; ++i) { // Copy directory names (strings) into dynamically allocated arrays
        (*stat)[i] = malloc(strlen(statList[i]) + 1);
        strcpy((*stat)[i], statList[i]);
        (*status)[i] = malloc(strlen(statusList[i]) + 1);
        strcpy((*status)[i], statusList[i]);
    }

    return numDir;
}

// Output of allocateProcs is procArray and numProc.
// procArray will be output as a dynamically allocated array which holds all ROOT processes.
// numDir represents the number of processes in procArray.
int allocateProcs(process *** procArray, char ** stat, char ** status, int numDir) {
    int numProc = 0;
    int uidList[1000];
    FILE * statFile = NULL;
    FILE * statusFile = NULL;

    int pid; char name[100];
    unsigned long int uTime, sTime;
    long int cuTime, csTime;
    unsigned long long int startTime;

    // The following for loop grabs process UIDs to determine which are ROOT processes
    for(int i = 0; i < numDir; ++i) {
        if( !(statusFile = fopen(status[i], "r")) ) { // Opening status file
            fprintf(stderr, "Could not open a status file.\n");
            exit(EXIT_FAILURE);
        }
        fscanf(statusFile,
               "%*s %*s %*s %*d %*s %*s %*s %*s %*d %*s %*d %*s %*d %*s %*d %*s %*d %*s %d",
               &uidList[i]
        );
        if(uidList[i] == 0) // Counting root processes
            ++numProc;
        fclose(statusFile);
    }

    *procArray = malloc(sizeof(process *) * numProc);
    for(int i = 0, j = 0; i < numDir; ++i) {
        if(uidList[i] != 0) // Filtering non-root processes
            continue;
        if( !(statFile = fopen(stat[i], "r")) ) {   // Opening stat file
            fprintf(stderr, "Could not open stat file.\n");
            exit(EXIT_FAILURE);
        }

        // The following lines copy into procArray
        (*procArray)[j] = malloc(sizeof(process));
        (*procArray)[j]->uid = uidList[i];
        
        // Temp variables are used so that fscanf modifier types match (otherwise errors occur while reading in data)
        fscanf(statFile,
               "%d %s %*c %*d %*d %*d %*d %*d %*u %*lu %*lu %*lu %*lu %lu %lu %ld %ld %*ld %*ld %*ld %*ld %llu",
               &pid, name, &uTime, &sTime, &cuTime, &csTime, &startTime  
        );
        strcpy(((*procArray)[j])->name, name); ((*procArray)[j])->pid = pid;
        ((*procArray)[j])->uTime = uTime; ((*procArray)[j])->sTime = sTime;
        ((*procArray)[j])->cuTime = cuTime; ((*procArray)[j])->csTime = csTime;
        ((*procArray)[j])->startTime = startTime;

        ++j; // Only increments on ROOT processes
        fclose(statFile);
    }
    return numProc;
}

// Return value communicates success/failure.
int calcCPU(process ** procArray, int size) {
    if(!procArray)
        return 0;

    long double Hz = sysconf(_SC_CLK_TCK);
    long double sysUpTime = 0;
    FILE * statFile = fopen("/proc/uptime", "r");
    fscanf(statFile, "%Lf", &sysUpTime);
    fclose(statFile);

    long double seconds = 0;
    for(int i = 0; i < size; ++i) {
        // Calculate total time for process
        procArray[i]->tTime = (procArray[i]->uTime + procArray[i]->sTime) / Hz;

        // Calculate total time w/ child process time included
        procArray[i]->ctTime = procArray[i]->tTime + ((procArray[i]->cuTime + procArray[i]->csTime) / Hz);
        //printf("%d:\n\ttTime=%Lf\nctTime=%Lf\n", i, procArray[i]->tTime, procArray[i]->ctTime);

        // Turn total elapsed time of process into units of seconds (google "jiffies")
        seconds = sysUpTime - (procArray[i]->startTime / Hz);

        // Calculate average % of cpu usage for this process
        procArray[i]->cpuPercent = 100 * (procArray[i]->tTime / seconds);
    }
    return 1;
}

// Return value communicates success/failure.
int displayAll(process ** procArray, int size) {
    if(!procArray)
        return 0;
    for(int i = 0; i < size; ++i) {
        printf("%d: ", i);
        if(!display(procArray[i])) {
            fprintf(stderr, "A process was found to be NULL unexpectedly.\n");
            exit(EXIT_FAILURE);
        }
    }
    return 1;
}

// Return value communicates success/failure.
int display(process * proc) { // Single process display
    if(!proc)
        return 0;
    printf("%s\n\tPID: %d\n\tAvg CPU Usage(%%): \t%Lf\n\tTotal CPU Time (s): \t%Lf\n\tw/ Children (s): \t%Lf\n",
            proc->name,
            proc->pid,
            proc->cpuPercent,
            proc->tTime,
            proc->ctTime);
    return 1;
}
