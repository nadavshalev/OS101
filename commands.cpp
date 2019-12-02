//		commands.c
//********************************************
#include "commands.h"
//********************************************
// function name: ExeCmd
 // Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
extern Job cjob;

int ExeCmd(list<Job*>& jobs, char* lineSize, char* lpwd, list<string>& history)
{			
	string cmdString = string(lineSize);
	char* cmd; 
	char* args[MAX_ARG];
	char pwd[MAX_LINE_SIZE];
	char* delimiters = " \t\n";  
	int i = 0, num_arg = 0;
	bool illegal_cmd = false; // illegal command

	// extract command from text-line
	cmd = strtok(lineSize, delimiters);
	if (cmd == NULL)
		return 0; 
   	args[0] = cmd;
	for (i=1; i<MAX_ARG; i++)
	{
		args[i] = strtok(NULL, delimiters); 
		if (args[i] != NULL) 
			num_arg++;
	}

    cjob.pid = 0;
    
	// update history buffer
	if (history.size()== HISTORY_SIZE+1){
   	    history.pop_front();
   	}
   	history.push_back(cmdString);

   	//remove copleted jobs
   	updateCompleteJobs(jobs);
/*************************************************/
// Built in Commands PLEASE NOTE NOT ALL REQUIRED
// ARE IN THIS CHAIN OF IF COMMANDS. PLEASE ADD
// MORE IF STATEMENTS AS REQUIRED
/*************************************************/
	if (!strcmp(cmd, "cd") ) 
	{	
		//get current dir
		getcwd(pwd, sizeof(pwd));
		// if last dir selected
		if (!strcmp(args[1], "-")){
			chdir(lpwd);
		}
		// if new dir selected
		else{
			int failure = chdir(args[1]);
			// fail to change dir
			if(failure){
				printf("smash error: > '%s' - path not found\n", args[1]);
				return 0;
			}
		}

		// set last dir and dir
		strncpy(lpwd, pwd, sizeof(pwd));
	} 
	
	/*************************************************/
	else if (!strcmp(cmd, "pwd")) 
	{
		getcwd(pwd, sizeof(pwd));
		printf("%s\n", pwd);
	}
	
	/*************************************************/
	else if (!strcmp(cmd, "mkdir"))
	{
 		
	}
	/*************************************************/
	
	else if (!strcmp(cmd, "jobs")) 
	{
	    list <Job*> :: iterator it;
	    int count = 0;
        for(it = jobs.begin(); it != jobs.end(); ++it){
            if(!((*it)->stop))
                cout << "[" << count+1 << "] " << (*it)->cmd << ": " << (*it)->pid << " " << time(0) - (*it)->startTime << "secs\n";
            else
                 cout << "[" << count+1 << "] " << (*it)->cmd << ": " << (*it)->pid << " " << time(0) - (*it)->startTime << "secs"<<" (stopped)\n";
            ++count;
        }
	}
	/*************************************************/
	else if (!strcmp(cmd, "kill"))
	{
	    int signum = -1 * stoi(args[1]);
	    int jobnum = stoi(args[2]);
	    if (jobnum > jobs.size()){
	        printf("‫‪smash‬‬ ‫‪error:‬‬ ‫>‬ ‫‪kill‬‬ ‫‪job‬‬ ‫–‬ ‫‪job‬‬ ‫‪does‬‬ ‫‪not‬‬ ‫‪exist\n‬‬");
	    }
	    else{
            list <Job*> :: iterator it;
            it = jobs.begin();
            for(int i = 1; i < jobnum;++i){
                ++it;
            }
            int pid = (*it)->pid;
            int result = kill(pid, signum);
             cout << "signal " << strsignal(signum) << " was sent to pid " << pid<<"\n" ;
            if (result != 0){
                printf("‫‪smash‬‬ ‫‪error:‬‬ ‫>‬ ‫‪kill‬‬ ‫‪job‬‬ ‫–‬ ‫‪cannot‬‬ ‫‪send‬‬ ‫‪signal\n‬‬");
            }
        }
	}
	/*************************************************/
	else if (!strcmp(cmd, "showpid")) 
	{
 		printf("smash pid is %d\n", getpid());
	}
	/*************************************************/
	else if (!strcmp(cmd, "fg"))
	{
	    list <Job*> :: iterator it;
	    int pid;
	    int jobnum;
	    if(!num_arg)
	        jobnum=1;
        else
	        int jobnum = stoi(args[0]);
	    it = jobs.begin();
        for(int i = 1; i < jobnum;++i){
                ++it;
        }
	    pid = (*it)->pid;
	    if((*it)->stop){
        cout << "signal " << strsignal(18) << " was sent to pid " << pid<<"\n" ;
        kill(pid, SIGCONT);
        }
        cout << (*it)->cmd<<"\n";
        int status;
        cjob = *(*it);
        jobs.erase(it++);
        pid_t result = waitpid(pid, &status, WUNTRACED);
        if (result == -1) {
            perror("Error while waiting for child proccess to end");
        }
        cjob.pid = 0;
    }

	/*************************************************/
	else if (!strcmp(cmd, "bg")) 
	{
  		
	}
	/*************************************************/
	else if (!strcmp(cmd, "history"))
	{
  		list <string> :: iterator it;
        for(it = history.begin(); it != --history.end(); ++it)
            cout << *it << '\n';
	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
   		
	} 
	/*************************************************/
	else // external command
	{
 		ExeExternal(args, cmdString);
	 	return 0;
	}
	if (illegal_cmd == true)
	{
		printf("smash error: > \"%s\"\n", cmdString);
		return 1;
	}
    return 0;
}
//**************************************************************************************
// function name: ExeExternal
// Description: executes external command
// Parameters: external command arguments, external command string
// Returns: void
//**************************************************************************************
void ExeExternal(char *args[MAX_ARG], string cmdString)
{
	int pID;
    	switch(pID = fork()) 
	{
    		case -1: 
					
					perror("Error while fork");
					break;
        	case 0 :
                	// Child Process
               		setpgrp();
					execvp(args[0], args);
					perror("Error in child proccess");
					exit(1);
					break;
			
			default:
					// wait for child to finish
                	int status;
			    	pid_t result = waitpid(pID, &status, WUNTRACED);
			    	cjob.pid = pID;
			    	cjob.cmd = cmdString;
			    	cjob.startTime = time(0);
			    	cjob.stop = false;
			        if (result == -1) {
			        	perror("Error while waiting for child proccess to end");
			        }
                    cjob.pid = 0;
					break;
	}
}
//**************************************************************************************
// function name: ExeComp
// Description: executes complicated command
// Parameters: command string
// Returns: 0- if complicated -1- if not
//**************************************************************************************
int ExeComp(char* lineSize)
{
	char ExtCmd[MAX_LINE_SIZE+2];
	char *args[MAX_ARG];
    if ((strstr(lineSize, "|")) || (strstr(lineSize, "<")) || (strstr(lineSize, ">")) || (strstr(lineSize, "*")) || (strstr(lineSize, "?")) || (strstr(lineSize, ">>")) || (strstr(lineSize, "|&")))
    {
		// Add your code here (execute a complicated command)
					
		/* 
		your code
		*/
	} 
	return -1;
}
//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
int BgCmd(char* lineSize, list<Job*>& jobs)
{

	char* Command;
	char* delimiters = " \t\n";
	char *args[MAX_ARG];
	if (lineSize[strlen(lineSize)-2] == '&')
	{
		lineSize[strlen(lineSize)-2] = '\0';
		cout << lineSize << "\n";
		char* cmd; 
		char* args[MAX_ARG];
		char* delimiters = " \t\n";  
		int i = 0;
	    	cmd = strtok(lineSize, delimiters);
		if (cmd == NULL)
			return 0; 
	   	args[0] = cmd;
		for (i=1; i<MAX_ARG; i++)
			args[i] = strtok(NULL, delimiters);

		int pID;
    	switch(pID = fork()) 
		{
    		case -1: 
					// Add your code here (error)
					
					perror("sume error");
					break;
        	case 0 :
                	// Child Process
               		setpgrp();
					execvp(cmd, args);
					perror("Error in background Proccess");
					printf("smash > ");
					exit(1);
					break;
			
			default:
					Job* jb = new Job;
					jb->pid = pID;
					jb->startTime = time(0);
					jb->cmd = string(lineSize);
					jb->stop = false;
            		jobs.push_front(jb);
            		return 0;
		}
		
	}
	return -1;
}

void updateCompleteJobs(list<Job*>& jobs){
	list <Job*> :: iterator it;
	it = jobs.begin();
    while (it != jobs.end()){
    	int status;
    	pid_t result = waitpid((*it)->pid, &status, WNOHANG);
        if (result != 0) {
        	cout << "remove job: " << (*it)->pid << "\n";
          	jobs.erase(it++);
        }
        else
        	it++;
    }
}

