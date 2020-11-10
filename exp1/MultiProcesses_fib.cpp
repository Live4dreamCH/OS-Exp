#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#define tasks 40
#define times 10

//经典折磨王:不优化地计算斐波那契数列
long fib(int n)
{
    if (n <= 2)
        return 1;
    return fib(n - 1) + fib(n - 2);
}

int main()
{
    pid_t fork_pid;
    int n = 40;
    long result = 0;
    //父进程循环创建子进程
    for (size_t i = 0; i < tasks; i++)
    {
        //每个子进程都会有一份独立的变量拷贝, 所以只要在fork前改变n的值,就可以将不同的参数传递给子进程
        // n = 40 + i;
        fork_pid = fork();
        //fork后的代码所有进程都可执行:如果是子进程就应该退出循环去执行计算任务;父进程就应该继续创建进程
        if (fork_pid == 0)
        {
            break;
        }
    }
    if (fork_pid == 0)
    {
        for (size_t i = 0; i < times; i++)
        {
            result += fib(n);
        }
        std::cout<<"my p_id="<<getpid()<< ", "<<times<<" * fib(" << n << ")=" << result << std::endl;
    }
    //主进程等待所有子进程
    else
    {
        for (size_t i = 0; i < tasks; i++)
        {
            wait(NULL);
        }
    }
    return 0;
}