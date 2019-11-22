#ifndef _COMMANDS_H
#define _COMMANDS_H
#include <unistd.h> 
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream> 
#include <list> 
#include <iterator> 
#include <ctime>
#define MAX_LINE_SIZE 80
#define MAX_ARG 20
using namespace std;
// typedef enum { FALSE , TRUE } bool;
struct Job
{
	int pid;
	time_t startTime;
};

int ExeComp(char* lineSize);
int BgCmd(char* lineSize, list<Job*>& jobs);
int ExeCmd(list<Job*>& jobs, char* lineSize, char* cmdString, char* lpwd);
void ExeExternal(char *args[MAX_ARG], char* cmdString);

#endif

