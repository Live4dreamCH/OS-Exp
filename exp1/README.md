# 第一次实验

## 目的:搭建环境,运行指定程序,并自己编写、运行一些程序

## 环境

本地:wsl-Ubuntu16.04  CPU:6core Mem:8GB  
> live4dream@lichenghao:~$ cat /proc/version  
> Linux version 4.4.0-18362-Microsoft (Microsoft@Microsoft.com) (gcc version 5.4.0 (GCC) ) #1049-Microsoft Thu Aug 14 12:01:00 PST 2020

之前就有的腾讯云:CentOS 7.5 64位  1core, 2GB, 1Mbps  
> [root@VM_0_13_centos ~]# cat /proc/version  
> Linux version 3.10.0-862.el7.x86_64 (builder@kbuilder.dev.centos.org) (gcc version 4.8.5 20150623 (Red Hat 4.8.5-28) (GCC) ) #1 SMP Fri Apr 20 16:44:24 UTC 2018

华为云:openEuler 20.03 64bit with ARM 2vCPUs | 4GB  
> [root@ecs-osexp ~]# cat /proc/version  
> Linux version 4.19.90-2003.4.0.0036.oe1.aarch64 (abuild@obs-worker-003) (gcc version 7.3.0 (GCC)) #1 SMP Mon Mar 23 19:06:43 UTC 2020

## 过程

在本地编写MultiProcesses.cpp(内容与指定程序基本一致)和makefile,  
过程中找不到wait()所在的头文件,搜索后解决:#include <sys/wait.h>
得到正确答案  
> live4dream@lichenghao:~/OS/exp1$ make
> g++ MultiProcesses.cpp -o MultiProcesses.out
> ./MultiProcesses.out
> main_pid=704
> I'm parent, fork()=>705, my pid=704
> I'm child, fork()=>0, my pid=705

新建GitHub仓库, 使用git在多端同步代码(较熟练, 未记录命令过程)

计划在本地编写MultiThreads.cpp, TrySocket.cpp, TryEpoll.cpp等

使用pthread, 参考各文档:

> [Linux manual](https://linux.die.net/man/3/pthread_create)
> [从pthread 到QThread](https://www.cnblogs.com/lfsblack/p/4614541.html)

在本地编写MultiThreads.cpp, 用多线程计算斐波那契数列(未优化版), 以充分利用多核资源, 并检验线程性能
运行成功:

> live4dream@lichenghao:~/OS/exp1$ make MultiThreads.out 
> g++ MultiThreads.cpp -pthread -o MultiThreads.out -Wall
> ./MultiThreads.out
> fib(now in main, pa->result[0]=10233415540)=102334155
> 
> now in main, pa->result[fib(1]=16558014141)=165580141
> 
> fib(now in main, pa->result[42)=267914296
> 2]=267914296
> now in main, pa->result[fib(3]=43349443743
> )=433494437
> now in main, pa->result[fib(4]=70140873344
> )=701408733
> fib(now in main, pa->result[45)=51134903170
> ]=1134903170
> fib(now in main, pa->result[466]=)=1836311903
> 1836311903
> fib(now in main, pa->result[47)=2971215073
> 7]=2971215073
> fib(now in main, pa->result[48)=48075269768
> ]=4807526976
> fib(now in main, pa->result[49)=7778742049
> 9]=7778742049

由于算出结果后主线程与计算线程竞争使用输出资源, 导致显示顺序有误. 可以对输出加锁,以保证输出串的完整性;
但实际上, 后期希望能够用这种方法比较进程、线程切换的开销, 所以目前主线程死循环轮询的方式一定会删去(这种轮询会占满一个内核, 给实验带来误差)

配置好云服务器, 连接成功:

> Welcome to 4.19.90-2003.4.0.0036.oe1.aarch64
> 
> System information as of time: 	Mon Nov  9 22:50:17 CST 2020
> 
> System load: 	0.00
> Processes: 	108
> Memory used: 	9.6%
> Swap used: 	0.0%
> Usage On: 	9%
> IP address: 	192.168.1.189
> Users online: 	1
> 
> [root@ecs-osexp ~]# python3 -V
> Python 3.7.4
> [root@ecs-osexp ~]# python -V
> Python 2.7.16
> [root@ecs-osexp ~]# git --version
> git version 2.23.0
> [root@ecs-osexp ~]# yum
> usage: yum [options] COMMAND
> [root@ecs-osexp ~]# cat /proc/version  
> Linux version 4.19.90-2003.4.0.0036.oe1.aarch64 (abuild@obs-worker-003) (gcc version 7.3.0 (GCC)) #1 SMP Mon Mar 23 19:06:43 UTC 2020

git导入代码