/*
 * Kobe Davis
 * Prof. Karavanic
 * CS 431
 * 8 March 2019
 *
 * -- Group Research Project: Detecting and Mitigating System Noise -- 
 * This program will read output from the cpuTime.c program.
 * Only the top 20 from each section will be seen because only the
 * top 20 are written to file from cpuTime.c.
 *
 * Usage:
 *      1. Compile: gcc -o read readCPU.c
 *      2. Run: ./read <filename>
 * 
 * Note: When writing the filename as a command line argument in step 2:
 *      1. Exclude angle-brackets
 *      2. Include file extension as part of the name
*/

#include <stdio.h>
#include <stdlib.h>

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

void display(process proc) {
    printf("%s\n\tPID: %d\n\tAvg CPU Usage(%%): \t%Lf\n\tTotal CPU Time (s): \t%Lf\n\tw/ Children (s): \t%Lf\n",
            proc.name,
            proc.pid,
            proc.cpuPercent,
            proc.tTime,
            proc.ctTime
    );
}

int main(int argc, char ** argv) {
    FILE * fp = fopen(argv[1], "r");
    fseek(fp, 0L, SEEK_END);
    int size = ftell(fp)/sizeof(process);
    rewind(fp);

    process procArray[size];
    fread(procArray, sizeof(process), size, fp);
    fclose(fp);

    printf("Finished scanning byte data.\n\n");
    for(int i = 0; i < size; ++i)
        display(procArray[i]);
    return 0;
}
