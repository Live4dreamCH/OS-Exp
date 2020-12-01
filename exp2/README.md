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

### 修改

将中断号改成14, sigalarm, 并用alarm()发起信号,程序运行正常.

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

> live4dream@lichenghao:~/OS/exp2$ make tube.out 
> gcc tube.c -o tube.out
> ./tube.out
> Child process 1 is sending message!
> Child process 2 is sending message!
> Child process 1 is sending message!
> Child process 2 is sending message!
> Child process 1 is sending message!
> Child process 2 is sending message!
> Child process 1 is sending message!
> Child process 2 is sending message!
> Child process 1 is sending message!
> Child process 2 is sending message!

分析:一个进程释放锁后稍微等一会,这个时候别的进程有机会申请到, 表现为并发执行,各进程响应时间都可以比较短

3. 一个谦让, 另一个不谦让(不sleep)

结果:(进程1是谦让的那个,进程2是不谦让的那个)

> live4dream@lichenghao:~/OS/exp2$ make tube.out 
> gcc tube.c -o tube.out
> ./tube.out
> Child process 1 is sending message!
> Child process 2 is sending message!
> Child process 2 is sending message!
> Child process 2 is sending message!
> Child process 2 is sending message!
> Child process 2 is sending message!
> Child process 1 is sending message!
> Child process 1 is sending message!
> Child process 1 is sending message!
> Child process 1 is sending message!

可以看到, 一旦进程2抢到了写入互斥锁, 1就不能抢到, 只能等2全部完成后再"吃剩饭".
这对1的运行不利, 也有失公平性.

### 修改:

将两个进程的写锁都注释掉, 输出仍然正常:

> live4dream@lichenghao:~/OS/exp2$ make tube.out 
> gcc tube.c -o tube.out
> ./tube.out
> Child process 1 is sending message!
> Child process 2 is sending message!
> Child process 2 is sending message!
> Child process 2 is sending message!
> Child process 2 is sending message!
> Child process 2 is sending message!
> Child process 1 is sending message!
> Child process 1 is sending message!
> Child process 1 is sending message!
> Child process 1 is sending message!

没有出现预期中的"脏数据"的情况. 在网上搜索原因, 得到的一个信息是:
[pipe 写入的原子性，是说多个进程写入管道，如果每次写入的字节数目都小于PIPE_BUF，那么各个进程写入的数据彼此不会交叠](https://bbs.csdn.net/topics/380209019)

## 页面置换

希望编写出一个交互式的命令-答复界面, 让使用者能够身临其境地感受不同算法之间的差别.

使用g++时, 报了大量的错:

> live4dream@lichenghao:~/OS/exp2$ make page.out 
> g++ page.cpp -o page.out -ggdb
> page.cpp: In member function ‘void FIFO::status()’:
> page.cpp:126:18: error: ‘it’ does not name a type
>          for(auto it = mem.begin();it!=mem.end();it++){
>                   ^

想到auto是c++11中加入的, 那就在编译时加入" -std==c++11"好了
结果失败
man g++后发现是" -std=c++11"
修改后正常

典型的使用情况如下:

1. FIFO:

> live4dream@lichenghao:~/OS/exp2$ ./page.out
> 输入内存最大页面数: 5
> 使用FIFO or LRU?(FIFO/LRU): FIFO
> help infos:可用指令如下
> [alloc | a] n: 分配n块页面;
> [write | w] n s: 向页面n写字符串s;
> [read | r] n: 读取页面n的内容;
> [delete | d] n: 删除页面n;
> [status | s]: 显示当前状态;
> [quit | q] n: 退出, 并显示命中率;
> [help | h | 其它] n: show this message;
> 
> \> a 7
> 已分配页面0, 初始化为空
> 已分配页面1, 初始化为空
> 已分配页面2, 初始化为空
> 已分配页面3, 初始化为空
> 已分配页面4, 初始化为空
> 页面0被调出内存
> 已分配页面5, 初始化为空
> 页面1被调出内存
> 已分配页面6, 初始化为空
> 已分配页号: 0~6供您使用
> 
> \> s
> 内存总页数: 5
> 已分配内存页数: 5, 页面号为:
> 2 3 4 5 6
> 在磁盘的页数: 2, 页面号为:
> 0 1
> FIFO队列中的页面号为:
> 2 3 4 5 6
> 
> \> r 0
> 页面2被调出内存
> 页面0被调回主存
> 页面0的内容为:
> 
> \> w 0 Hello
> 已将 Hello 写入页面0中!
> 
> \> r 0
> 页面0的内容为: Hello
> 
> \> w 1 World
> 页面3被调出内存
> 页面1被调回主存
> 已将 World 写入页面1中!
> 
> \> s
> 内存总页数: 5
> 已分配内存页数: 5, 页面号为:
> 0 1 4 5 6
> 在磁盘的页数: 2, 页面号为:
> 2 3
> FIFO队列中的页面号为:
> 4 5 6 0 1
> 
> \> d 2
> 页面4被调出内存
> 页面2被调回主存
> 页面2被删除了
> 
> \> s
> 内存总页数: 5
> 已分配内存页数: 4, 页面号为:
> 0 1 5 6
> 在磁盘的页数: 2, 页面号为:
> 3 4
> FIFO队列中的页面号为:
> 5 6 0 1
> 
> \> q
> 操作次数: 12, 调入次数: 3命中率: 75%

2. LRU:

> live4dream@lichenghao:~/OS/exp2$ ./page.out
> 输入内存最大页面数: 5
> 使用FIFO or LRU?(FIFO/LRU): LRU
> help infos:可用指令如下
> [alloc | a] n: 分配n块页面;
> [write | w] n s: 向页面n写字符串s;
> [read | r] n: 读取页面n的内容;
> [delete | d] n: 删除页面n;
> [status | s]: 显示当前状态;
> [quit | q] n: 退出, 并显示命中率;
> [help | h | 其它] n: show this message;
> 
> \> a 7
> 已分配页面0, 初始化为空
> 已分配页面1, 初始化为空
> 已分配页面2, 初始化为空
> 已分配页面3, 初始化为空
> 已分配页面4, 初始化为空
> 页面0被调出内存
> 已分配页面5, 初始化为空
> 页面1被调出内存
> 已分配页面6, 初始化为空
> 已分配页号: 0~6供您使用
> 
> \> s
> 内存总页数: 5
> 已分配内存页数: 5, 页面号为:
> 2 3 4 5 6
> 在磁盘的页数: 2, 页面号为:
> 0 1
> LRU队列中的页面号为:
> 2 3 4 5 6
> 
> \> r 0
> 页面2被调出内存
> 页面0被调回主存
> 已将页面0的LRU值改变!
> 页面0的内容为:
> 
> \> w 0 Hello
> 已将页面0的LRU值改变!
> 已将 Hello 写入页面0中!
> 
> \> r 0
> 已将页面0的LRU值改变!
> 页面0的内容为: Hello
> 
> \> w 1 World
> 页面3被调出内存
> 页面1被调回主存
> 已将页面1的LRU值改变!
> 已将 World 写入页面1中!
> 
> \> s
> 内存总页数: 5
> 已分配内存页数: 5, 页面号为:
> 0 1 4 5 6
> 在磁盘的页数: 2, 页面号为:
> 2 3
> LRU队列中的页面号为:
> 4 5 6 0 1
> 
> \> d 2
> 页面4被调出内存
> 页面2被调回主存
> 页面2被删除了
> 
> \> s
> 内存总页数: 5
> 已分配内存页数: 4, 页面号为:
> 0 1 5 6
> 在磁盘的页数: 2, 页面号为:
> 3 4
> LRU队列中的页面号为:
> 5 6 0 1
> 
> \> q
> 操作次数: 12, 调入次数: 3命中率: 75%

### 修改

与要求不相符合, 需要添加一个序列访问的功能

已添加access功能, 可以方便比较在局部性前提下的算法性能

1. FIFO:

> live4dream@lichenghao:~/OS/exp2$ ./page.out
> 输入内存最大页面数: 5
> 使用FIFO or LRU?(FIFO/LRU): FIFO
> help infos:可用指令如下
> [alloc | a] n: 分配n块页面;
> [write | w] n s: 向页面n写字符串s;
> [read | r] n: 读取页面n的内容;
> [access] n n*[num]: 对一系列[num]页面, 进行n次访问;
> [delete | d] n: 删除页面n;
> [status | s]: 显示当前状态;
> [quit | q] n: 退出, 并显示命中率;
> [help | h | 其它] n: show this message;
> 
> \> a 7
> 已分配页面0, 初始化为空
> 已分配页面1, 初始化为空
> 已分配页面2, 初始化为空
> 已分配页面3, 初始化为空
> 已分配页面4, 初始化为空
> 页面0被调出内存
> 已分配页面5, 初始化为空
> 页面1被调出内存
> 已分配页面6, 初始化为空
> 已分配页号: 0~6供您使用
> 
> \> access 15 0 1 0 2 0 3 0 4 0 5 0 6 0 3 0
> 页面2被调出内存
> 页面0被调回主存
> 页面0的内容为:
> 页面3被调出内存
> 页面1被调回主存
> 页面1的内容为:
> 页面0的内容为:
> 页面4被调出内存
> 页面2被调回主存
> 页面2的内容为:
> 页面0的内容为:
> 页面5被调出内存
> 页面3被调回主存
> 页面3的内容为:
> 页面0的内容为:
> 页面6被调出内存
> 页面4被调回主存
> 页面4的内容为:
> 页面0的内容为:
> 页面0被调出内存
> 页面5被调回主存
> 页面5的内容为:
> 页面1被调出内存
> 页面0被调回主存
> 页面0的内容为:
> 页面2被调出内存
> 页面6被调回主存
> 页面6的内容为:
> 页面0的内容为:
> 页面3的内容为:
> 页面0的内容为:
> 
> \> q
> 操作次数: 22, 调入次数: 8命中率: 63.6364%

2. LRU:

> live4dream@lichenghao:~/OS/exp2$ ./page.out
> 输入内存最大页面数: 5
> 使用FIFO or LRU?(FIFO/LRU): LRU
> help infos:可用指令如下
> [alloc | a] n: 分配n块页面;
> [write | w] n s: 向页面n写字符串s;
> [read | r] n: 读取页面n的内容;
> [access] n n*[num]: 对一系列[num]页面, 进行n次访问;
> [delete | d] n: 删除页面n;
> [status | s]: 显示当前状态;
> [quit | q] n: 退出, 并显示命中率;
> [help | h | 其它] n: show this message;
> 
> \> a 7
> 已分配页面0, 初始化为空
> 已分配页面1, 初始化为空
> 已分配页面2, 初始化为空
> 已分配页面3, 初始化为空
> 已分配页面4, 初始化为空
> 页面0被调出内存
> 已分配页面5, 初始化为空
> 页面1被调出内存
> 已分配页面6, 初始化为空
> 已分配页号: 0~6供您使用
> 
> \> access 15 0 1 0 2 0 3 0 4 0 5 0 6 0 3 0
> 页面2被调出内存
> 页面0被调回主存
> 已将页面0的LRU值改变!
> 页面0的内容为:
> 页面3被调出内存
> 页面1被调回主存
> 已将页面1的LRU值改变!
> 页面1的内容为:
> 已将页面0的LRU值改变!
> 页面0的内容为:
> 页面4被调出内存
> 页面2被调回主存
> 已将页面2的LRU值改变!
> 页面2的内容为:
> 已将页面0的LRU值改变!
> 页面0的内容为:
> 页面5被调出内存
> 页面3被调回主存
> 已将页面3的LRU值改变!
> 页面3的内容为:
> 已将页面0的LRU值改变!
> 页面0的内容为:
> 页面6被调出内存
> 页面4被调回主存
> 已将页面4的LRU值改变!
> 页面4的内容为:
> 已将页面0的LRU值改变!
> 页面0的内容为:
> 页面1被调出内存
> 页面5被调回主存
> 已将页面5的LRU值改变!
> 页面5的内容为:
> 已将页面0的LRU值改变!
> 页面0的内容为:
> 页面2被调出内存
> 页面6被调回主存
> 已将页面6的LRU值改变!
> 页面6的内容为:
> 已将页面0的LRU值改变!
> 页面0的内容为:
> 已将页面3的LRU值改变!
> 页面3的内容为:
> 已将页面0的LRU值改变!
> 页面0的内容为:
> 
> \> q
> 操作次数: 22, 调入次数: 7命中率: 68.1818%

结论:在考虑程序局部性的前提下,LRU的命中率通常好于FIFO
