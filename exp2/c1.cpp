#include <iostream>
#include <queue>
using namespace std;

const int ap = 5;
const int pp = 4;
const int total_instruction = 12;

int page[ap] = {0};
int pagecontrol[pp] = {0};
int main_p[total_instruction] = {3, 2, 1, 0, 3, 2, 4, 3, 2, 1, 0, 4};
int diseffect = 0;
queue<int> fifo;

int isEmpty()
{
    for (int i = 0; i < pp; i++)
    {
        if (pagecontrol[i] == -1)
        {
            return i;
        }
    }
    return -1;
}
int main()
{
    for (int i = 0; i < ap; i++)
    {
        page[i] = -1;
    }
    for (int i = 0; i < pp; i++)
    {
        pagecontrol[i] = -1;
    }
    // init end
    for (auto i : main_p)
    {
        // cout << "page:" << i << ",State:" << page[i] << endl;
        if (page[i] == -1)
        {
            // 未命中
            diseffect++;
            cout << "diseffect:" << diseffect << endl;
            int position = isEmpty();
            if (position == -1)
            {
                int page_po = fifo.front();
                int pagecontrol_po = page[page_po];
                cout << "page was replaced:" << page_po << endl;
                fifo.pop();
                page[page_po] = -1;
                pagecontrol[pagecontrol_po] = -1;

                page[i] = pagecontrol_po;
                pagecontrol[pagecontrol_po] = i;
                fifo.push(i);
            }
            else
            {

                page[i] = position;
                pagecontrol[position] = i;
                fifo.push(i);
            }
        }
        else
        {
            // 命中
            // continue;
        }
        // cout << endl;
        cout << endl;
    }
    cout << "rate:" << 1 - diseffect * 1.0 /total_instruction << endl;
    return 0;
}