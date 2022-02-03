#include <stdio.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <signal.h>
#include <sys/syscall.h>
#include <sys/user.h>

#define MAX_SIZE 400

void readBannedSysCalls(char *filename, int *array) {
	FILE *fp = fopen(filename, "r");
	
	int sysCall = -1;
	while (fscanf(fp, "%d\n", &sysCall) != EOF)
		array[sysCall] = 1;
	
	fclose(fp);
}

int main(int argc, char **argv) {
	int bannedSysCalls[MAX_SIZE] = {0};	
	
	if (argc < 2) {
		fprintf(stderr, "Insufficient number of arguments given. Exiting...\n");
		exit(-1);
	}
	
	readBannedSysCalls(argv[1], bannedSysCalls);
	
	pid_t pid;
	
	pid = fork();
	
	if (pid == 0) {
		ptrace(PTRACE_TRACEME, 0, NULL, NULL);
		execl("test", "test", (char *) NULL);
	}
	else {
		int status;			
		struct user_regs_struct regs;
		int uniqueSysCallsCalled[MAX_SIZE] = {0};
		int enterSysCall = 1;
		int blockedSysCalls = 0;
		int uniqueSysCalls = 0;
		int calledSysCalls = 0;
		
		while (1){			
			wait(&status);
			
			if (WIFSTOPPED(status) && WSTOPSIG(status) == SIGTRAP){
				ptrace(PTRACE_GETREGS, pid, 0, &regs);
							
				if (bannedSysCalls[regs.orig_eax]) {
					blockedSysCalls++;
					regs.orig_eax = -1;
					ptrace(PTRACE_SETREGS, pid, 0, &regs);					
				}
				if (enterSysCall) {					
					if (!uniqueSysCallsCalled[regs.orig_eax]) { 
						uniqueSysCalls++;
						uniqueSysCallsCalled[regs.orig_eax] = 1;							
					}
					
					calledSysCalls++;					
					enterSysCall = 0;
				}
				else
					enterSysCall = 1;
			}
			if (WIFEXITED(status) || WIFSIGNALED(status)){
				break;
			}

			ptrace(PTRACE_SYSCALL, pid, 0, 0);
		}
		printf("%d System Call(s) Prohibited\n", blockedSysCalls);
		printf("%d Unique System Call(s) Called\n", uniqueSysCalls);
		printf("%d System Call(s) Called\n", calledSysCalls);
		
	}
}