#ifndef _SIGS_H
#define _SIGS_H
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

using namespace std;
void signal_Ctrl_C( int signum );
void signal_Ctrl_Z( int signum );

#endif

