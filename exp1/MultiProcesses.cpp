#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
// #include <pthread.h>
// #include <sys/epoll.h>

int main()
{
    std::cout<<"main_pid="<<getpid()<<std::endl;
    pid_t fork_pid;
    fork_pid = fork();
    if (fork_pid == 0)
    {
        std::cout << "I'm child, fork()=>0, my pid=" << getpid() << std::endl;
    }
    else if (fork_pid > 0)
    {
        std::cout << "I'm parent, fork()=>" << fork_pid << ", my pid=" << getpid() << std::endl;
        wait(0);
    }
    else
    {
        std::cout << "wrong? fork()=>" << fork_pid << ", my pid=" << getpid() << std::endl;
    }
    return 0;
}