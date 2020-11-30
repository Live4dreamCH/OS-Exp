#include <sys/socket.h>
#include <iostream>
#include <sys/types.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

void print_error(char *msg)
{
    std::cerr << msg << " ; errno = " << errno;
    exit(errno);
}

void setnonblocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    int r = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    if (r != 0)
        print_error("setnonblocking");
}

int main(int argc, char *argv[])
{
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == -1)
    {
        print_error("creat socket");
    }
    // setnonblocking(s);
    connect(s, )
    return 0;
}