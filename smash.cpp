/*	smash.c
main file. This file contains the main function of smash
*******************************************************************/
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <iostream> 
#include <list> 
#include <iterator> 
#include <ctime>
#include "commands.h"
#include "signals.h"
#define MAX_LINE_SIZE 80
#define MAXARGS 20

using namespace std;

char* L_Fg_Cmd;
list <Job*> jobs; //This represents the list of jobs. Please change to a preferred type (e.g array of char*)
char lineSize[MAX_LINE_SIZE];
Job cjob;
//**************************************************************************************
// function name: main
// Description: main function of smash. get command from user and calls command functions
//**************************************************************************************
int main(int argc, char *argv[])
{
    string cmdString;

	char lpwd[MAX_LINE_SIZE]; // save last pwd
	getcwd(lpwd, sizeof(lpwd)); // init as current pwd
	list <string> history;

	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	//set your signal handlers here
	/* add your code here */

	/************************************/
	signal(SIGINT, signal_Ctrl_C);
	signal(SIGTSTP, signal_Ctrl_Z);
	/************************************/
	// Init globals 


	
	L_Fg_Cmd =(char*)malloc(sizeof(char)*(MAX_LINE_SIZE+1));
	if (L_Fg_Cmd == NULL) 
			exit (-1); 
	L_Fg_Cmd[0] = '\0';
	
    	while (true)
    	{
		 	printf("smash > ");
			fgets(lineSize, MAX_LINE_SIZE, stdin);
						// perform a complicated Command
			if(!ExeComp(lineSize)) continue; 
						// background command	
		 	if(!BgCmd(lineSize, jobs)) continue; 
						// built in commands
			ExeCmd(jobs, lineSize, lpwd, history);
			
			/* initialize for next line read*/
			lineSize[0]='\0';
			cmdString[0]='\0';
		}
    return 0;
}

