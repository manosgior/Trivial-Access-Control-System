#include <stdio.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <signal.h>
#include <sys/syscall.h>
#include <sys/user.h>

int main() {
	pid_t pid;
	
	pid = fork();
	
	if (pid == 0) {
		ptrace(PTRACE_TRACEME, 0, NULL, NULL);
		execl("test", "test", (char *) NULL);
	}
	else {
		int status;			
		struct user_regs_struct regs;
		
		while(1){
			//ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_TRACEFORK);
			wait(&status);
			
			if(WIFSTOPPED(status) && WSTOPSIG(status) == SIGTRAP){
				ptrace(PTRACE_GETREGS, pid, 0, &regs);
					
				if (regs.orig_eax ==  __NR_clone || regs.orig_eax ==  __NR_fork) {
					regs.orig_eax = -1;
					ptrace(PTRACE_SETREGS, pid, 0, &regs);
				}
				
			}
			if(WIFEXITED(status) || WIFSIGNALED(status)){
				break;
			}

			ptrace(PTRACE_SYSCALL, pid, 0, 0);
		}	
	}
}