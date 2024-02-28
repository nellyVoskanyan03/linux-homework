#include <sys/ucontext.h>
#include <sys/types.h>
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <pwd.h>

void sigusr1_handler(int _sig, siginfo_t *_info, void *_context) {
       
    pid_t pid = _info->si_pid;

    ucontext_t *context = (ucontext_t *)_context;
   
    uid_t uid = _info->si_uid;
    struct passwd *pwd = getpwuid(uid);
    const char *username =  pwd->pw_name;
  
    unsigned long eip = context->uc_mcontext.gregs[REG_RIP];
    unsigned long eax = context->uc_mcontext.gregs[REG_RAX];
    unsigned long ebx = context->uc_mcontext.gregs[REG_RBX];

    std::cout << "Received a SIGUSR1 signal from process " << pid << " executed by " << uid << " (" << username << ")." << std::endl;
    std::cout << "State of the context: EIP = " << eip << ", EAX = " << eax << ", EBX = " << ebx << "." << std::endl;
}

int main() {

    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_sigaction = sigusr1_handler;
    action.sa_flags = SA_SIGINFO;
    
    int sig_result = sigaction(SIGUSR1, &action, NULL);
    if (sig_result < 0) {
        std::cerr << strerror(errno) << std::endl;
        exit(errno);
    }
 
    pid_t pid = getpid();
    std::cout << "PID : " << pid << std::endl;

    
    while (true) {
        sleep(2);
    }

    return 0;
}