#include <iostream>
#include <deque>
#include <map>
#include <string>

class FIFO
{
private:
    int instruct_count, diseffect;
    std::deque<int> order;
    std::map<int, std::string> mem, disk;
    int mem_max_page, next_page;

public:
    FIFO(int max) : mem_max_page(max), next_page(0), instruct_count(0), diseffect(0) {}
    void alloc(int n)
    {
        int temp = next_page;

        for (; next_page < temp + n; next_page++)
        {
            if (mem.size() == mem_max_page)
            {
                mem2disk(1);
            }
            mem[next_page] = "";
            order.push_back(next_page);
            std::cout << "已分配页面" << next_page << ", 初始化为空\n";
        }
        instruct_count += n;
        std::cout << "已分配页号: " << temp << '~' << next_page - 1 << "供您使用\n";
    }
    int mem2disk(int n)
    {
        if (n > mem.size())
            n = mem.size();
        int temp;
        for (int i = 0; i < n; i++)
        {
            temp = order.front();
            order.pop_front();
            disk[temp] = mem[temp];
            mem.erase(temp);
            std::cout << "页面" << temp << "被调出内存\n";
        }
        return mem_max_page - mem.size();
    }
    bool disk2mem(int i)
    {
        if (disk.find(i) == disk.end())
        {
            std::cout << "磁盘中无页面" << i << "!\n";
            return false;
        }
        if (mem.size() == mem_max_page)
        {
            mem2disk(1);
        }
        mem[i] = disk[i];
        disk.erase(i);
        order.push_back(i);
        diseffect++;
        std::cout << "页面" << i << "被调回主存\n";
        return true;
    }
    bool del(int i)
    {
        if (mem.find(i) == mem.end())
        {
            if (disk.find(i) == disk.end())
            {
                std::cout << "没有页面" << i << "需要删除!\n";
                return false;
            }
            disk2mem(i);
        }
        mem.erase(i);

        bool del_flag = false;
        int sz = order.size() - 1;
        for (int j = 0; j < sz; j++)
        {
            if (order[j] == i)
            {
                del_flag = true;
            }
            if (del_flag)
            {
                order[j] = order[j + 1];
            }
        }
        order.pop_back();

        instruct_count++;
        std::cout << "页面" << i << "被删除了\n";
        return true;
    }
    void write(int i, std::string s)
    {
        if (mem.find(i) == mem.end())
        {
            if (disk.find(i) == disk.end())
            {
                std::cout << "没有页面" << i << "可供写入!\n";
            }
            else
            {
                disk2mem(i);
            }
        }
        mem[i] = s;
        instruct_count++;
        std::cout << "已将 " << s << " 写入页面" << i << "中!\n";
    }
    void read(int i)
    {
        if (mem.find(i) == mem.end())
        {
            if (disk.find(i) == disk.end())
            {
                std::cout << "没有页面" << i << "可读取!\n";
            }
            else
            {
                disk2mem(i);
            }
        }
        instruct_count++;
        std::cout << "页面" << i << "的内容为: " << mem[i] << "\n";
    }
    void status()
    {
        std::cout << "内存总页数: " << mem_max_page << "\n";
        std::cout << "已分配内存页数: " << mem.size() << ", 页面号为:\n";
        for (auto it = mem.begin(); it != mem.end(); it++)
        {
            std::cout << it->first << ' ';
        }
        std::cout << "\n在磁盘的页数: " << disk.size() << ", 页面号为:\n";
        for (auto it = disk.begin(); it != disk.end(); it++)
        {
            std::cout << it->first << ' ';
        }
        std::cout << "\nFIFO队列中的页面号为: \n";
        for (auto it = order.begin(); it != order.end(); it++)
        {
            std::cout << *it << ' ';
        }
        std::cout << '\n';
    }
    void quit()
    {
        std::cout << "操作次数: " << instruct_count << ", 调入次数: " << diseffect << "命中率: " << (1 - diseffect / (double)instruct_count) * 100 << "%\n";
    }
    void help()
    {
        std::cout << "help infos:可用指令如下\n";
        std::cout << "[alloc | a] n: 分配n块页面;\n";
        std::cout << "[write | w] n s: 向页面n写字符串s;\n";
        std::cout << "[read | r] n: 读取页面n的内容;\n";
        std::cout << "[access] n n*[num]: 对一系列[num]页面, 进行n次访问;\n";
        std::cout << "[delete | d] n: 删除页面n;\n";
        std::cout << "[status | s]: 显示当前状态;\n";
        std::cout << "[quit | q] n: 退出, 并显示命中率;\n";
        std::cout << "[help | h | 其它] n: show this message;\n";
    }
};

class LRU
{
private:
    int instruct_count, diseffect;
    std::deque<int> order;
    std::map<int, std::string> mem, disk;
    int mem_max_page, next_page;

public:
    LRU(int max) : mem_max_page(max), next_page(0), instruct_count(0), diseffect(0) {}
    void alloc(int n)
    {
        int temp = next_page;

        for (; next_page < temp + n; next_page++)
        {
            if (mem.size() == mem_max_page)
            {
                mem2disk(1);
            }
            mem[next_page] = "";
            order.push_back(next_page);
            std::cout << "已分配页面" << next_page << ", 初始化为空\n";
        }
        instruct_count += n;
        std::cout << "已分配页号: " << temp << '~' << next_page - 1 << "供您使用\n";
    }
    int mem2disk(int n)
    {
        if (n > mem.size())
            n = mem.size();
        int temp;
        for (int i = 0; i < n; i++)
        {
            temp = order.front();
            order.pop_front();
            disk[temp] = mem[temp];
            mem.erase(temp);
            std::cout << "页面" << temp << "被调出内存\n";
        }
        return mem_max_page - mem.size();
    }
    bool disk2mem(int i)
    {
        if (disk.find(i) == disk.end())
        {
            std::cout << "磁盘中无页面" << i << "!\n";
            return false;
        }
        if (mem.size() == mem_max_page)
        {
            mem2disk(1);
        }
        mem[i] = disk[i];
        disk.erase(i);
        order.push_back(i);
        diseffect++;
        std::cout << "页面" << i << "被调回主存\n";
        return true;
    }
    bool del(int i)
    {
        if (mem.find(i) == mem.end())
        {
            if (disk.find(i) == disk.end())
            {
                std::cout << "没有页面" << i << "需要删除!\n";
                return false;
            }
            disk2mem(i);
        }
        mem.erase(i);

        bool del_flag = false;
        int sz = order.size() - 1;
        for (int j = 0; j < sz; j++)
        {
            if (order[j] == i)
            {
                del_flag = true;
            }
            if (del_flag)
            {
                order[j] = order[j + 1];
            }
        }
        order.pop_back();

        instruct_count++;
        std::cout << "页面" << i << "被删除了\n";
        return true;
    }
    void write(int i, std::string s)
    {
        if (mem.find(i) == mem.end())
        {
            if (disk.find(i) == disk.end())
            {
                std::cout << "没有页面" << i << "可供写入!\n";
            }
            else
            {
                disk2mem(i);
            }
        }
        mem[i] = s;
        update(i);
        instruct_count++;
        std::cout << "已将 " << s << " 写入页面" << i << "中!\n";
    }
    void read(int i)
    {
        if (mem.find(i) == mem.end())
        {
            if (disk.find(i) == disk.end())
            {
                std::cout << "没有页面" << i << "可读取!\n";
            }
            else
            {
                disk2mem(i);
            }
        }
        update(i);
        instruct_count++;
        std::cout << "页面" << i << "的内容为: " << mem[i] << "\n";
    }

    void update(int i)
    {
        for (auto it = order.begin(); it != order.end(); it++)
        {
            if (*it == i)
            {
                order.erase(it);
                break;
            }
        }
        order.push_back(i);
        std::cout << "已将页面" << i << "的LRU值改变!\n";
    }

    void status()
    {
        std::cout << "内存总页数: " << mem_max_page << "\n";
        std::cout << "已分配内存页数: " << mem.size() << ", 页面号为:\n";
        for (auto it = mem.begin(); it != mem.end(); it++)
        {
            std::cout << it->first << ' ';
        }
        std::cout << "\n在磁盘的页数: " << disk.size() << ", 页面号为:\n";
        for (auto it = disk.begin(); it != disk.end(); it++)
        {
            std::cout << it->first << ' ';
        }
        std::cout << "\nLRU队列中的页面号为: \n";
        for (auto it = order.begin(); it != order.end(); it++)
        {
            std::cout << *it << ' ';
        }
        std::cout << '\n';
    }
    void quit()
    {
        std::cout << "操作次数: " << instruct_count << ", 调入次数: " << diseffect << "命中率: " << (1 - diseffect / (double)instruct_count) * 100 << "%\n";
    }
    void help()
    {
        std::cout << "help infos:可用指令如下\n";
        std::cout << "[alloc | a] n: 分配n块页面;\n";
        std::cout << "[write | w] n s: 向页面n写字符串s;\n";
        std::cout << "[read | r] n: 读取页面n的内容;\n";
        std::cout << "[access] n n*[num]: 对一系列[num]页面, 进行n次访问;\n";
        std::cout << "[delete | d] n: 删除页面n;\n";
        std::cout << "[status | s]: 显示当前状态;\n";
        std::cout << "[quit | q] n: 退出, 并显示命中率;\n";
        std::cout << "[help | h | 其它] n: show this message;\n";
    }
};

int main()
{
    std::string re;
    int maxpage;
    std::cout << "输入内存最大页面数: ";
    std::cin >> maxpage;
    std::cout << "使用FIFO or LRU?(FIFO/LRU): ";
    std::cin >> re;
    if (re == "FIFO")
    {
        FIFO test(maxpage);
        std::string op, s;
        int n;
        test.help();
        while (true)
        {
            std::cout << "\n> ";
            std::cin >> op;
            if (op == "alloc" || op == "a")
            {
                std::cin >> n;
                test.alloc(n);
            }
            else if (op == "write" || op == "w")
            {
                std::cin >> n >> s;
                test.write(n, s);
            }
            else if (op == "read" || op == "r")
            {
                std::cin >> n;
                test.read(n);
            }
            else if (op == "access")
            {
                std::cin >> n;
                int temp;
                for(int i=0;i<n;i++){
                    std::cin>>temp;
                    test.read(temp);
                }
            }
            else if (op == "delete" || op == "d")
            {
                std::cin >> n;
                test.del(n);
            }
            else if (op == "status" || op == "s")
            {
                test.status();
            }
            else if (op == "quit" || op == "q")
            {
                test.quit();
                break;
            }
            else
            {
                test.help();
            }
        }
    }
    else{
        LRU test(maxpage);
        std::string op, s;
        int n;
        test.help();
        while (true)
        {
            std::cout << "\n> ";
            std::cin >> op;
            if (op == "alloc" || op == "a")
            {
                std::cin >> n;
                test.alloc(n);
            }
            else if (op == "write" || op == "w")
            {
                std::cin >> n >> s;
                test.write(n, s);
            }
            else if (op == "read" || op == "r")
            {
                std::cin >> n;
                test.read(n);
            }
            else if (op == "access")
            {
                std::cin >> n;
                int temp;
                for(int i=0;i<n;i++){
                    std::cin>>temp;
                    test.read(temp);
                }
            }
            else if (op == "delete" || op == "d")
            {
                std::cin >> n;
                test.del(n);
            }
            else if (op == "status" || op == "s")
            {
                test.status();
            }
            else if (op == "quit" || op == "q")
            {
                test.quit();
                break;
            }
            else
            {
                test.help();
            }
        }
    }

    return 0;
}