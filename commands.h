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
#define HISTORY_SIZE 50
using namespace std;

struct Job
{
	int pid=0;
	time_t startTime;
	string cmd;
	bool stop;
};

void updateCompleteJobs(list<Job*>& jobs);
void quitJob(Job* job, int id);
void freeJobs(list<Job*> jobs);
int ExeComp(char* lineSize);
int BgCmd(char* lineSize, list<Job*>& jobs);
int ExeCmd(list<Job*>& jobs, char* lineSize, char* lpwd, list<string>& history);
void ExeExternal(char *args[MAX_ARG], string cmdString);

#endif

