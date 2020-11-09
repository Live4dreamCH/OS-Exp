#include <iostream>

int square(int a)
{
    return a * a;
}

struct para
{
    int n;
    long result;
};

int main()
{
    struct para hh[10];
    std::cout<<sizeof(hh)/sizeof(struct para)<<std::endl;
    int a = 10, b;
    int *pa = &a, *pb = &b;
    int (*ps)(int);
    ps = &square;
    // b = square(a);
    // *pb = square(*pa);
    *pb = (*ps)(*pa);
    std::cout << "a=" << a << ", b=" << b << '\n';
    return 0;
}