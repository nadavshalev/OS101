// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.h"
#include "commands.h"

extern Job cjob;
extern list <Job*> jobs;

void signal_Ctrl_Z( int signum ){
    if (cjob.pid != 0){
        cout << "\nsignal SIGSTOP was sent to pid " << cjob.pid <<"\n" ;
        int result = kill(cjob.pid, SIGSTOP);
        if(result == 0){
            Job* jb = new Job;
            *jb = cjob;
            jb->stop = true;
            cjob.pid = 0;
            jobs.push_back(jb);
        }
        else
            perror("SIGSTOP failed");
    }
}

void signal_Ctrl_C( int signum ){
	if (cjob.pid != 0){
        cout << "\nsignal SIGINT was sent to pid " << cjob.pid <<"\n" ;
        kill(cjob.pid, SIGINT);
    }
}