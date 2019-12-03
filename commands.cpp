//		commands.c
//********************************************
#include "commands.h"
//********************************************
// function name: ExeCmd
 // Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string, last pwd, history
// Returns: 0 - success,1 - failure
//********************************************
extern Job cjob;

int ExeCmd(list<Job*>& jobs, char* lineSize, char* lpwd, list<string>& history)
{			
	string cmdString = string(lineSize);
	cmdString = cmdString.substr(0, cmdString.size()-1);
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
	    if (!num_arg){
            cout << "smash error: > \"" << cmdString << "\"\n";
            return 0;
        }
        if (num_arg > 1){
            cout << "smash error: > \"" << cmdString << "\"\n";
            return 0;
        }
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
                cout << "smash error: > \"" << args[1] << "\" - path not found\n";
				return 0;
			}
		}

		// set last dir and dir
		strncpy(lpwd, pwd, sizeof(pwd));
	} 
	
	/*************************************************/
	else if (!strcmp(cmd, "pwd")) 
	{
	    if (!num_arg){
            getcwd(pwd, sizeof(pwd));
            printf("%s\n", pwd);
		}
		else
            cout << "smash error: > \"" << cmdString << "\"\n";
	}

	/*************************************************/
	else if (!strcmp(cmd, "mv"))
	{
        int result = rename(args[1], args[2]);
        if(result == 0)
            cout << args[1] << " has been renamed to " << args[2] << "\n";
        else
            perror("can't rename file");

	}
	/*************************************************/
	
	else if (!strcmp(cmd, "jobs")) 
	{
	    if (!num_arg){
            list <Job*> :: iterator it;
            int count = 0;
            for(it = jobs.begin(); it != jobs.end(); ++it){
                if(!((*it)->stop))
                    cout << "[" << count+1 << "] " << (*it)->cmd << " : " << (*it)->pid << " " << time(0) - (*it)->startTime << " secs\n";
                else
                    cout << "[" << count+1 << "] " << (*it)->cmd << " : " << (*it)->pid << " " << time(0) - (*it)->startTime << " secs"<<" (stopped)\n";
                ++count;
            }
        }
        else
            cout << "smash error: > \"" << cmdString << "\"\n";

	}
	/*************************************************/
	else if (!strcmp(cmd, "kill"))
	{
	    try {
	        if (num_arg > 2){
                cout << "smash error: > \"" << cmdString << "\"\n";
                return 0;
            }
            int signum = -1 * stoi(args[1]);
            int jobnum = stoi(args[2]);
            if (jobnum > jobs.size()){
                cout << "‫‪smash‬‬ ‫‪error:‬‬ ‫>‬ ‫‪kill‬‬ " << jobnum << " ‫–‬ ‫‪job‬‬ ‫‪does‬‬ ‫‪not‬‬ ‫‪exist\n‬‬";
                return 0;
            }

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
        catch (const exception& e) {
            cout << "smash error: > \"" << cmdString << "\"\n";
        }
	}
	/*************************************************/
	else if (!strcmp(cmd, "showpid")) 
	{
	    if (!num_arg)
 		    printf("smash pid is %d\n", getpid());
        else
            cout << "smash error: > \"" << cmdString << "\"\n";
	}
	/*************************************************/
	else if (!strcmp(cmd, "fg"))
	{
        if (num_arg > 1){
            cout << "smash error: > \"" << cmdString << "\"\n";
            return 0;
        }
	    list <Job*> :: iterator it;
	    int pid;
	    int jobnum;
	    if(!num_arg)
	        jobnum=1;
        else{
             try{
	            jobnum = stoi(args[1]);
            }
            catch(const std::exception& e){
                cout << "smash error: > \"" << cmdString << "\"\n";
                return 0;
            }
         }

        if (jobnum > jobs.size()){
            cout << "‫‪smash‬‬ ‫‪error:‬‬ ‫>‬ bg " << jobnum << " ‫–‬ ‫‪job‬‬ ‫‪does‬‬ ‫‪not‬‬ ‫‪exist\n‬‬";
            return 0;
        }
	    it = jobs.begin();
        for(int i = 1; i < jobnum;++i){
            ++it;
        }
        if(it == jobs.end()){
            cout << "smash error: > \"" << cmdString << "\"\n";
            return 0;
        }
	    pid = (*it)->pid;
	    if((*it)->stop){
            cout << "signal SIGCONT was sent to pid " << pid<<"\n" ;
            kill(pid, SIGCONT);
        }
        cout << (*it)->cmd<<"\n";
        int status;
        cjob = *(*it);
        free((*it));
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
	    if (num_arg > 1){
            cout << "smash error: > \"" << cmdString << "\"\n";
            return 0;
        }
  		list <Job*> :: iterator it;
	    int pid;
	    int jobnum;
	    bool isOK = false;
	    if(!num_arg){
	    	it = jobs.end();
        	while(it != jobs.begin()){
        		it--;
	            if((*it)->stop){
	                isOK = true;
	                break;
                }
	        }
	    }
        else{
            try{
        	  	it = jobs.begin();
	            int jobnum = stoi(args[1]);
                if (jobnum > jobs.size()){
                    cout << "‫‪smash‬‬ ‫‪error:‬‬ ‫>‬ bg " << jobnum << " ‫–‬ ‫‪job‬‬ ‫‪does‬‬ ‫‪not‬‬ ‫‪exist\n‬‬";
                    return 0;
                }
                for(int i = 1; i < jobnum;++i){
                    ++it;
                }
                if ((*it)->stop)
                    isOK = true;
            }
            catch(const std::exception& e){
                cout << "smash error: > \"" << cmdString << "\"\n";
            }
        }
        if(isOK){
            pid = (*it)->pid;
            cout << "signal SIGCONT was sent to pid " << pid<<"\n" ;
            kill(pid, SIGCONT);
            (*it)->stop = false;
            cout << (*it)->cmd << "\n";
            int status;
        }
	}
	/*************************************************/
	else if (!strcmp(cmd, "history"))
	{
	    if (num_arg > 0){
            cout << "smash error: > \"" << cmdString << "\"\n";
            return 0;
        }
  		list <string> :: iterator it;
  		// loop history. doesnt show current command
        for(it = history.begin(); it != --history.end(); ++it)
            cout << *it << '\n';
	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
	    if (num_arg == 0){
            freeJobs(jobs);
            exit(0);
	    }

	    if(!strcmp(args[1],"kill")){
	        list <Job*> :: iterator it;
	        int id = 0;
	        // loop all jobs and send kill signals
	        for(it = jobs.begin(); it != jobs.end(); ++it){
                quitJob((*it),id+1);
                id++;
            }
            freeJobs(jobs);
            exit(0);
	    }
        else
            cout << "smash error: > \"" << cmdString << "\"\n";
	} 
	/*************************************************/
	else // external command
	{
 		ExeExternal(args, cmdString);
	 	return 0;
	}
	if (illegal_cmd)
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
					// add current job
                	int status;
                	cjob.pid = pID;
			    	cjob.cmd = cmdString;
			    	cjob.startTime = time(0);
			    	cjob.stop = false;
			    	pid_t result = waitpid(pID, &status, WUNTRACED);

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
		return 0;
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
		string cmdString = string(lineSize);
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
	                freeJobs(jobs);
					exit(1);
					break;
			
			default:
					// add job to list
					Job* jb = new Job;
					jb->pid = pID;
					jb->startTime = time(0);
					jb->cmd = cmdString;
					jb->stop = false;
            		jobs.push_back(jb);
            		return 0;
		}
		
	}
	return -1;
}

//********************************************
// function name: updateCompleteJobs
 // Description: update and remove Completed Jobs
// Parameters: jobs list
//********************************************
void updateCompleteJobs(list<Job*>& jobs){
	list <Job*> :: iterator it;
	it = jobs.begin();
	// loop all jobs
    while (it != jobs.end()){
    	int status;
    	pid_t result = waitpid((*it)->pid, &status, WNOHANG);
        if (result != 0) {
            free((*it));
          	jobs.erase(it++);
        }
        else
        	it++;
    }
}

//********************************************
// function name: quitJob
// Description: send kill to job
// Parameters: pointer to job, job id
//********************************************
void quitJob(Job* job, int id){
    kill(job->pid, SIGTERM);
    if(id > 0)
        cout << "[" << id << "] " << job->cmd << " - Sending SIGTERM...";
    time_t sTime = time(0);
    // wait to terminate
    int pStat = 0, status;
    while(pStat == 0){
        pStat = waitpid(job->pid, &status, WNOHANG);
        if (time(0)-sTime > 5){
            if(id > 0)
                cout << "(5 seconds passed) Sending SIGKILL...";
            kill(job->pid, SIGKILL);
            break;
        }
    }
    cout << "Done.\n";
}

//********************************************
// function name: freeJobs
// Description: free Job object data
// Parameters: pointer to job, job id
//********************************************
void freeJobs(list<Job*> jobs){
    list <Job*> :: iterator it;
    for(it = jobs.begin(); it != jobs.end(); ++it){
       free((*it));
    }
}

