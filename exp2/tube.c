#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int pid1, pid2; // 定义两个进程变量
int main()
{
    int fd[2];
    char OutPipe[100], InPipe[100]; // 定义两个字符数组
    pipe(fd);
    lockf(fd[1], 0, 0);
    while ((pid1 = fork()) == -1)
        ; // 如果进程1创建不成功,则空循环
    if (pid1 == 0)
    { // 如果子进程1创建成功
        //sleep(2);
        sprintf(OutPipe, "Child process 1 is sending message!\n"); // 给Outpipe赋值
        for (int i = 0; i < 5; i++)
        {
            lockf(fd[1], 1, 0);        // 锁定管道
            write(fd[1], OutPipe, 50); // 向管道写入数据
            lockf(fd[1], 0, 0);        // 解除管道的锁定
            sleep(1);
        }
        // sleep(5); // 等待读进程读出数据
        exit(0); // 结束进程1
    }
    else
    {
        while ((pid2 = fork()) == -1)
            ; // 若进程2创建不成功,则空循环
        if (pid2 == 0)
        {
            sprintf(OutPipe, "Child process 2 is sending message!\n");
            for (int i = 0; i < 5; i++)
            {
                lockf(fd[1], 1, 0);
                write(fd[1], OutPipe, 50); // 向管道写入数据
                lockf(fd[1], 0, 0);
                sleep(0);
            }
            // sleep(5);
            exit(0);
        }
        else
        {
            //waitpid(pid1,0,0);                   // 等待子进程1 结束
            wait(0);
            // read(fd[0], InPipe, 50); // 从管道中读出数据
            // printf("%s\n", InPipe);	 // 显示读出的数据
            wait(0); // 等待子进程2 结束
            // read(fd[0], InPipe, 50); // 从管道中读出数据
            // printf("%s\n", InPipe);
            for (int i = 0; i < 10; i++)
            {
                read(fd[0], InPipe, 50); // 从管道中读出数据
                printf("%s", InPipe);
            }
            exit(0); // 父进程结束
        }
    }
}
