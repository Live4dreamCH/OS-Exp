#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#define tasks 40
#define times 10

// 给线程执行的函数传参数n和pthread_id, 以及信号量end用于主线程退出
struct para
{
    pthread_t id;
    sem_t *end;
    int n;
};

//经典折磨王:不优化地计算斐波那契数列
long fib(int n)
{
    if (n <= 2)
        return 1;
    return fib(n - 1) + fib(n - 2);
}

// 线程执行的函数
void *do_something(void *arg)
{
    struct para *val = (struct para *)arg;
    long result;
    for (size_t i = 0; i < times; i++)
    {
        result += fib(val->n);
    }
    std::cout <<"my pthread_id="<<val->id<< ", "<<times<<" * fib(" << val->n << ")=" << result << std::endl;
    sem_post(val->end);
    return NULL;
}

int main(void)
{
    pthread_t id;        //线程代号
    pthread_attr_t attr; //线程属性
    int ret;             //存pthread_create()返回值, 看创建线程是否成功
    // int flag = 0;
    struct para pa[tasks]; //给各线程传的参
    sem_t sem;
    sem_init(&(sem), 0, 0);//初始化信号量
    for (size_t i = 0; i < tasks; i++)
    {
        pa[i].n = 40;
        pa[i].end = &sem;
    }
    

    //设置属性,让新线程与主线程解绑
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    //创建线程
    for (size_t i = 0; i < tasks; i++)
    {
        ret = pthread_create(&id, &attr, &do_something, &pa[i]);
        pa[i].id = id;
        //创建失败
        if (ret != 0)
        {
            std::cout << "Create pthread " << i << " error!\n";
            return 1;
        }
    }

    //销毁属性
    pthread_attr_destroy(&attr);

    // 创建成功, 主线程轮询结果
    // while (flag != tasks)
    // {
    //     sleep(1);
    //     for (size_t i = 0; i < tasks; i++)
    //     {
    //         if (pa[i].result != 0)
    //         {
    //             std::cout << "now in main, pa->result[" << i << "]=" << pa[i].result << std::endl;
    //             flag++;
    //             pa[i].result = 0;
    //         }
    //     }
    // }

    //如果不解绑, 新线程结束时, 需要主线程帮它释放资源,就得用join等它
    //此处等待的是最后一个线程, 它的计算时间最长
    // pthread_join(id, NULL);
    //如果不等会怎么样?主线程创建完10个子线程就干完活了, 然后请求系统释放自己(整个进程), 于是子线程没来得及计算完成就随着整个进程被系统清理掉了
    //于是运行此程序, 什么都没有打印

    //用信号量同步主线程与子线程的退出
    for (size_t i = 0; i < tasks; i++)
    {
        sem_wait(&sem);
    }
    
    return 0;
}
