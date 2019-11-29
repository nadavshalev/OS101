// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.h"

void signal_Ctrl_Z( int signum ){
	cout << "ctrl+z\n";
}

void signal_Ctrl_C( int signum ){
	cout << "ctrl+c\n";
	exit(0);
}