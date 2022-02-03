# Trivial-Access-Control-System

Trivial access control system for system calls. This program restricts an executable (tracee) from invoking certain syscalls which receives them as input from a text file. The text file must contain the codes of the restricted syscalls. 


There are two variations:  
-The first simply uses LD_PRELOAD to load a shared library with a custom implementation of the syscall  
-The second interrupts the restricted syscall, logs it and then proceeds with the execution as if it never happened.  
