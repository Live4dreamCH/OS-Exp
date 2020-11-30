#include <iostream>
#include <queue>
using namespace std;

int ap = 5;
int pp = 3;
const int ap_const = 5;
const int pp_const = 3;
const int total_instruction = 100;

int page[ap_const] = {0};
int pagecontrol[pp_const] = {0};
int main_p[total_instruction] = {0};
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
    for (int i = 0; i < ap_const; i++)
    {
        page[i] = -1;
    }
    for (int i = 0; i < pp_const; i++)
    {
        pagecontrol[i] = -1;
    }
    // init end
    cout<<"page:";
    cin >> ap;
    cout<<"pagecontrol:";
    cin >> pp;
    cout << endl;
    for (int kidding = 0; kidding < total_instruction; kidding++)
    {
        int i;
        cin >> i;
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
        cout << "rate:" << 1 - diseffect * 1.0 / (kidding+1) << endl;
        cout << endl;
    }
    return 0;
}