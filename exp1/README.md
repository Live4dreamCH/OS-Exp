# 第一次实验

## 目的:搭建环境,运行指定程序,并自己编写、运行一些程序

## 环境

本地:wsl-Ubuntu16.04  CPU:6core 12虚拟cpu Mem:8GB  
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
所以不用加锁

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
本地:

> live4dream@lichenghao:~/OS$ git status
> On branch main
> Your branch is up-to-date with 'origin/main'.
> Changes not staged for commit:
>   (use "git add <file>..." to update what will be committed)
>   (use "git checkout -- <file>..." to discard changes in working directory)
> 
>         modified:   exp1/MultiProcesses.cpp
>         modified:   exp1/README.md
>         modified:   exp1/makefile
> 
> Untracked files:
>   (use "git add <file>..." to include in what will be committed)
> 
>         .gitignore
>         exp1/MultiThreads.cpp
>         exp1/example.cpp
>         exp1/pointer.cpp
> 
> no changes added to commit (use "git add" and/or "git commit -a")
> live4dream@lichenghao:~/OS$ git add *
> live4dream@lichenghao:~/OS$ git commit -m "初步多线程"
> [main 56feafe] 初步多线程
>  6 files changed, 360 insertions(+), 12 deletions(-)
>  create mode 100644 exp1/MultiThreads.cpp
>  create mode 100644 exp1/example.cpp
>  rewrite exp1/makefile (70%)
>  create mode 100644 exp1/pointer.cpp
> live4dream@lichenghao:~/OS$ git push
> warning: push.default is unset; its implicit value has changed in
> Git 2.0 from 'matching' to 'simple'. To squelch this message
> and maintain the traditional behavior, use:
> 
>   git config --global push.default matching
> 
> To squelch this message and adopt the new behavior now, use:
> 
>   git config --global push.default simple
> 
> When push.default is set to 'matching', git will push local branches
> to the remote branches that already exist with the same name.
> 
> Since Git 2.0, Git defaults to the more conservative 'simple'
> behavior, which only pushes the current branch to the corresponding
> remote branch that 'git pull' uses to update the current branch.
> 
> See 'git help config' and search for 'push.default' for further information.
> (the 'simple' mode was introduced in Git 1.7.11. Use the similar mode
> 'current' instead of 'simple' if you sometimes use older versions of Git)
> 
> Counting objects: 9, done.
> Delta compression using up to 12 threads.
> Compressing objects: 100% (8/8), done.
> Writing objects: 100% (9/9), 4.81 KiB | 0 bytes/s, done.
> Total 9 (delta 1), reused 0 (delta 0)
> remote: Resolving deltas: 100% (1/1), completed with 1 local object.
> To git@github.com:Live4dreamCH/OS-Exp.git
>    a4353e3..56feafe  main -> main
> live4dream@lichenghao:~/OS$ git config --global push.default simple
> live4dream@lichenghao:~/OS$ git status
> On branch main
> Your branch is up-to-date with 'origin/main'.
> Untracked files:
>   (use "git add <file>..." to include in what will be committed)
> 
>         .gitignore
> 
> nothing added to commit but untracked files present (use "git add" to track)
> live4dream@lichenghao:~/OS$ git add .gitignore 
> live4dream@lichenghao:~/OS$ git commit -m "添加.gitignore"
> [main b851585] 添加.gitignore
>  1 file changed, 2 insertions(+)
>  create mode 100644 .gitignore
> live4dream@lichenghao:~/OS$ git push
> Counting objects: 3, done.
> Delta compression using up to 12 threads.
> Compressing objects: 100% (2/2), done.
> Writing objects: 100% (3/3), 309 bytes | 0 bytes/s, done.
> Total 3 (delta 0), reused 0 (delta 0)
> To git@github.com:Live4dreamCH/OS-Exp.git
>    56feafe..b851585  main -> main

在云端, clone代码:

> [root@ecs-osexp ~]# git clone git@github.com:Live4dreamCH/OS-Exp.git
> Cloning into 'OS-Exp'...
> The authenticity of host 'github.com (192.30.255.113)' can't be established.
> RSA key fingerprint is SHA256:nThbg6kXUpJWGl7E1IGOCspRomTxdCARLviKw6E5SY8.
> Are you sure you want to continue connecting (yes/no)? yes
> Warning: Permanently added 'github.com,192.30.255.113' (RSA) to the list of known hosts.
> git@github.com: Permission denied (publickey).
> fatal: Could not read from remote repository.
> 
> Please make sure you have the correct access rights
> and the repository exists.

由于仓库私有, 云端无法访问, 需要先配置账户信息:  
[服务器上的 Git - 生成 SSH 公钥](https://git-scm.com/book/zh/v2/%E6%9C%8D%E5%8A%A1%E5%99%A8%E4%B8%8A%E7%9A%84-Git-%E7%94%9F%E6%88%90-SSH-%E5%85%AC%E9%92%A5)
先生成SSH公钥, 再在GitHub上添加它, 为安全性, 此过程不记录命令行

> [root@ecs-osexp ~]# git clone git@github.com:Live4dreamCH/OS-Exp.git
> Cloning into 'OS-Exp'...
> Warning: Permanently added the RSA host key for IP address '192.30.255.112' to the list of known hosts.
> remote: Enumerating objects: 18, done.
> remote: Counting objects: 100% (18/18), done.
> remote: Compressing objects: 100% (13/13), done.
> remote: Total 18 (delta 1), reused 18 (delta 1), pack-reused 0
> Receiving objects: 100% (18/18), 5.72 KiB | 5.72 MiB/s, done.
> Resolving deltas: 100% (1/1), done.
> [root@ecs-osexp ~]# ls
> OS-Exp

clone完成, 在云上跑一下程序吧:

> [root@ecs-osexp OS-Exp]# cd exp1/
> [root@ecs-osexp exp1]# ls
> example.cpp  makefile  MultiProcesses.cpp  MultiThreads.cpp  pointer.cpp  README.md
> [root@ecs-osexp exp1]# make --version
> GNU Make 4.2.1
> Built for aarch64-openEuler-linux-gnu
> Copyright (C) 1988-2016 Free Software Foundation, Inc.
> License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
> This is free software: you are free to change and redistribute it.
> There is NO WARRANTY, to the extent permitted by law.
> [root@ecs-osexp exp1]# make MultiProcesses.out
> g++ MultiProcesses.cpp -o MultiProcesses.out -Wall
> ./MultiProcesses.out
> main_pid=7780
> I'm parent, fork()=>7781, my pid=7780
> I'm child, fork()=>0, my pid=7781
> [root@ecs-osexp exp1]# make MultiThreads.out
> g++ MultiThreads.cpp -pthread -o MultiThreads.out -Wall
> ./MultiThreads.out
> fib(40)=102334155
> now in main, pa->result[0]=102334155
> fib(41)=165580141
> now in main, pa->result[1]=165580141
> fib(42)=267914296
> now in main, pa->result[2]=267914296
> fib(43)=433494437
> now in main, pa->result[3]=433494437
> now in main, pa->result[fib(4]=701408733
> 44)=0
> fib(now in main, pa->result[45)=5]=11349031701134903170
> 
> fib(46)=1836311903
> now in main, pa->result[6]=1836311903
> fib(47)=2971215073
> now in main, pa->result[7]=2971215073
> fib(now in main, pa->result[488)=]=48075269764807526976
> 
> now in main, pa->result[fib(949]=)=77787420497778742049

结果与本地一致.

已修改MultiThreads, 主线程等待执行时间最长的线程结束.

> [root@ecs-osexp exp1]# make MultiThreads.out
> g++ MultiThreads.cpp -pthread -o MultiThreads.out -Wall
> ./MultiThreads.out
> fib(40)=102334155
> fib(41)=165580141
> fib(42)=267914296
> fib(43)=433494437
> fib(44)=701408733
> fib(45)=1134903170
> fib(46)=1836311903
> fib(47)=2971215073
> fib(48)=4807526976
> fib(49)=7778742049

此时没有输出资源的争夺了.

验收反馈：老师希望能够在实验报告里添加多线程与多进程对比的目的、做法、体会等

下一步计划：多进程执行fib, 加入计时功能, 对比线程进程性能, 写报告

计时好方法:使用time执行命令:

> live4dream@lichenghao:~/OS/exp1$ time ls
> example.cpp  makefile  MultiProcesses.cpp  MultiProcesses_fib.cpp  MultiThreads.cpp  MultiThreads.out  pointer.cpp  README.md
> 
> real    0m0.009s
> user    0m0.000s
> sys     0m0.000s

real是wall clock的时长, 即从开始运行到结束的时间差
user是用户态进程占用cpu的时长,不包括io等时长
sys是内核态消耗的CPU时间
参见[Linux下clock计时函数学习](https://www.cnblogs.com/wfwenchao/p/5195022.html)

对多线程与多进程(每个程序分别新建40个线程/进程,每个线程/进程分别计算10次fib(40)的值并累加)分别计时,得到如下结果:

live4dream@lichenghao:~/OS/exp1$ make compare
g++ MultiThreads.cpp -pthread -o MultiThreads.out -Wall
time -v ./MultiThreads.out
my pthread_id=139794623760128, 10 * fib(40)=1023341550
...
类似输出共40行,为节省空间,此处只保留首尾两行
...
my pthread_id=139794640668416, 10 * fib(40)=1023341550
        Command being timed: "./MultiThreads.out"
        User time (seconds): 295.12
        System time (seconds): 0.01
        Percent of CPU this job got: 1192%
        Elapsed (wall clock) time (h:mm:ss or m:ss): 0:24.74
        Average shared text size (kbytes): 0
        Average unshared data size (kbytes): 0
        Average stack size (kbytes): 0
        Average total size (kbytes): 0
        Maximum resident set size (kbytes): 1832
        Average resident set size (kbytes): 0
        Major (requiring I/O) page faults: 0
        Minor (reclaiming a frame) page faults: 559
        Voluntary context switches: 0
        Involuntary context switches: 0
        Swaps: 0
        File system inputs: 0
        File system outputs: 0
        Socket messages sent: 0
        Socket messages received: 0
        Signals delivered: 0
        Page size (bytes): 4096
        Exit status: 0
g++ MultiProcesses_fib.cpp -o MultiProcesses_fib.out -Wall
time -v ./MultiProcesses_fib.out
my p_id=5991, 10 * fib(40)=1023341550
...
类似输出共40行,为节省空间,此处只保留首尾两行
...
my p_id=6017, 10 * fib(40)=1023341550
        Command being timed: "./MultiProcesses_fib.out"
        User time (seconds): 300.45
        System time (seconds): 0.10
        Percent of CPU this job got: 1156%
        Elapsed (wall clock) time (h:mm:ss or m:ss): 0:25.98
        Average shared text size (kbytes): 0
        Average unshared data size (kbytes): 0
        Average stack size (kbytes): 0
        Average total size (kbytes): 0
        Maximum resident set size (kbytes): 1360
        Average resident set size (kbytes): 0
        Major (requiring I/O) page faults: 0
        Minor (reclaiming a frame) page faults: 7870
        Voluntary context switches: 0
        Involuntary context switches: 0
        Swaps: 0
        File system inputs: 0
        File system outputs: 0
        Socket messages sent: 0
        Socket messages received: 0
        Signals delivered: 0
        Page size (bytes): 4096
        Exit status: 0

可以看到:  
在执行相同计算任务时, 多线程在各项数据上均优于多进程:  
内核态运行时间是多进程的1/10;  
在12个虚拟内核的电脑上跑40个线程/进程, 30秒左右的总执行时间,两者之间差距有一秒多.而这个差距还将随着线程数/进程数的增加而不断加大.  
事实上, 在2000年左右, 当时的互联网服务经常为了服务一个用户而新建一个