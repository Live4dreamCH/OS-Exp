#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#define fibs 10

// 给线程执行的函数传参数、或者把返回值带回到主线程
struct para
{
    int n;
    long result;
};

//经典折磨王
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
    val->result = fib(val->n);
    std::cout << "fib(" << val->n << ")=" << val->result << std::endl;
    return NULL;
}

int main(void)
{
    pthread_t id;        //线程代号
    pthread_attr_t attr; //线程属性
    int ret;             //存pthread_create()返回值, 看创建线程是否成功
    int flag = 0;
    struct para pa[fibs]; //给各线程传的参
    for (size_t i = 0; i < fibs; i++)
    {
        pa[i].n = 40 + i;
        pa[i].result = 0;
    }

    //设置属性,让新线程与主线程解绑
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    //创建线程
    for (size_t i = 0; i < fibs; i++)
    {
        ret = pthread_create(&id, &attr, &do_something, &pa[i]);
        //创建失败
        if (ret != 0)
        {
            std::cout << "Create pthread " << i << " error!\n";
            return 1;
        }
    }

    //销毁属性
    pthread_attr_destroy(&attr);

    //创建成功, 主线程轮询结果
    while (flag != fibs)
    {
        for (size_t i = 0; i < fibs; i++)
        {
            if (pa[i].result != 0)
            {
                std::cout << "now in main, pa->result[" << i << "]=" << pa[i].result << std::endl;
                flag++;
                pa[i].result = 0;
            }
        }
    }
    //如果不解绑, 新线程结束时, 需要主线程帮它释放资源,就得等它
    // pthread_join(id, NULL);
    return 0;
}
