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
    cout << "SIGSTOP\n";
    if (!cjob.pid){
        cout << cjob.pid << " | " << cjob.cmd << " |\n";
//        int result = kill(cjob.pid, SIGSTOP);
//        if(result){
//            Job* jb = new Job;
//            *jb = cjob;
//            jb->stop = true;
//            cjob.pid = 0;
//            jobs.push_front(jb);
//        }
    }
}

void signal_Ctrl_C( int signum ){
	cout << "ctrl+c\n";
	exit(0);
}