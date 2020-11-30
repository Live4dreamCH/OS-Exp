# 实验二

## 软中断

编写makefile...
编写源代码...

由于3号sigquit的按键CTRL+\被其它应用占用了, 无法触发,故更换成2号sigint, 触发键CTRL+C
程序终于调试成功, 观察到了各种现象:

1. 键盘触发中断, 2先1后

> live4dream@lichenghao:~/OS/exp2$ ./sig.out
> ^Cget signal
> get signal
> get signal
> get signal
> get signal
> get signal
> get signal
> 
> 
> Child process2 is killed by parent!!
> Child process1 is killed by parent!!
> 
> Parent process is killed!!

2. 键盘触发中断, 1先2后

> live4dream@lichenghao:~/OS/exp2$ ./sig.out
> ^Cget signal
> get signal
> get signal
> get signal
> get signal
> get signal
> get signal
> 
> 
> Child process1 is killed by parent!!
> Child process2 is killed by parent!!
> 
> Parent process is killed!!

3. 等待5秒结束, 1先2后

> live4dream@lichenghao:~/OS/exp2$ ./sig.out
> get signal
> get signal
> 
> 
> Child process1 is killed by parent!!
> Child process2 is killed by parent!!
> 
> Parent process is killed!!

4. 等待5秒结束, 2先1后

> live4dream@lichenghao:~/OS/exp2$ ./sig.out
> get signal
> get signal
> 
> 
> Child process2 is killed by parent!!
> Child process1 is killed by parent!!
> 
> Parent process is killed!!

推测的原因:子进程接收到软中断前, 正在进行忙等.
处理软中断需要执行完当前一条忙等语句, 然后跳转到stop中, 修改wait_flag, 跳出忙等, 打印这行语句.
此过程中:

1. 单核cpu下,系统调度顺序不同导致打印有先后,且先后顺序不固定

2. 多核cpu下,各核负载情况不同,将此任务轮转执行的时机不同, 导致打印先后不固定.

## 管道

多个进程对管道输入/输出时需要互斥, 我着重探索其中情况.

1. 互不相让, 能占就占

结果:

> live4dream@lichenghao:~/OS/exp2$ make tube.out 
> gcc tube.c -o tube.out
> ./tube.out
> Child process 1 is sending message!
> Child process 1 is sending message!
> Child process 1 is sending message!
> Child process 1 is sending message!
> Child process 1 is sending message!
> Child process 2 is sending message!
> Child process 2 is sending message!
> Child process 2 is sending message!
> Child process 2 is sending message!
> Child process 2 is sending message!

分析:进程1先拿到锁, 由于释放锁后不等待就继续下一次循环, 1一直抢先申请锁,导致2一直抢不到,这种情况可能造成2响应时间极长,应当避免

2. 互相谦让, 释放锁后都sleep一下

结果:

live4dream@lichenghao:~/OS/exp2$ make tube.out 
gcc tube.c -o tube.out
./tube.out
Child process 1 is sending message!
Child process 2 is sending message!
Child process 1 is sending message!
Child process 2 is sending message!
Child process 1 is sending message!
Child process 2 is sending message!
Child process 1 is sending message!
Child process 2 is sending message!
Child process 1 is sending message!
Child process 2 is sending message!

分析:一个进程释放锁后稍微等一会,这个时候别的进程有机会申请到, 表现为并发执行,各进程响应时间都可以比较短

3. 一个谦让, 另一个不谦让(不sleep)

结果:(进程1是谦让的那个,进程2是不谦让的那个)

live4dream@lichenghao:~/OS/exp2$ make tube.out 
gcc tube.c -o tube.out
./tube.out
Child process 1 is sending message!
Child process 2 is sending message!
Child process 2 is sending message!
Child process 2 is sending message!
Child process 2 is sending message!
Child process 2 is sending message!
Child process 1 is sending message!
Child process 1 is sending message!
Child process 1 is sending message!
Child process 1 is sending message!

可以看到, 一旦进程2抢到了写入互斥锁, 1就不能抢到, 只能等2全部完成后再"吃剩饭".
这对1的运行不利, 也有失公平性.

## 页面置换

使用g++时, 报了大量的错:

live4dream@lichenghao:~/OS/exp2$ make page.out 
g++ page.cpp -o page.out -ggdb
page.cpp: In member function ‘void FIFO::status()’:
page.cpp:126:18: error: ‘it’ does not name a type
         for(auto it = mem.begin();it!=mem.end();it++){
                  ^

想到auto是c++11中加入的, 那就在编译时加入" -std==c++11"好了
结果失败
man g++后发现是" -std=c++11"
修改后正常
