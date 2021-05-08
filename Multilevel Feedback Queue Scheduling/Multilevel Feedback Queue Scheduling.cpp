//多级队列调度算法
#include <iostream>
#include <fstream>
#define RQ1_n 5
#define RQ2_n 5
using namespace std;
struct tag_pcb
{
    string name;
    int need;
    int turn;
    int wait;
    tag_pcb *next;
    tag_pcb() { turn = 0; }
};

class PCB
{

private:
    tag_pcb *RQ1, *RQ2;
    int clock;
    int q;
    int s_wait[RQ1_n];
    int s_need[RQ2_n];

public:
    PCB()
    {
        RQ1 = new tag_pcb;
        RQ2 = new tag_pcb;
        clock = 0;
        q = 7;
        Create();
    }
    void Create(); //创建队列(通过读取txt文件)
    void Insertsort(tag_pcb *first, int *t);
    void Charge_RQ1();
    void Charge_RQ2();
    void Show();
};
void PCB::Create()
{
    ifstream input;
    input.open("Source.txt");
    int j = 0;
    tag_pcb *p = RQ1;
    tag_pcb *q = RQ2;
    for (j = 0; j < 10; j++)
    {
        tag_pcb *r = new tag_pcb;
        if (j < RQ1_n)
        {
            input >> p->name;
            if (j < RQ1_n - 1)
            {
                p->next = r;
                p = p->next;
            }
            else
                p->next = RQ1;
        }
        else
        {
            input >> q->name;
            if (j < 9)
            {
                q->next = r;
                q = q->next;
            }
            else
                q->next = RQ2;
        }
    }
    p = RQ1;
    q = RQ2;
    for (j = 0; j < 10; j++)
    {
        if (j < RQ1_n)
        {
            input >> p->need;
            p = p->next;
        }
        else
        {
            input >> q->need;
            s_need[j - RQ1_n] = q->need;
            q = q->next;
        }
    }
    p = RQ1;
    q = RQ2;
    int turn = 0;
    for (j = 0; j < 10; j++)
    {
        if (j < RQ1_n)
        {
            input >> p->wait;
            s_wait[j] = -p->wait;
            p = p->next;
        }
        else
        {
            input >> q->wait;
            q = q->next;
        }
    }
    input.close();
    Insertsort(RQ1, s_wait);
    Insertsort(RQ2, s_need);
    Charge_RQ1();
    Charge_RQ2();
    Show();
}
void PCB::Show()
{
    tag_pcb *p = RQ1;
    tag_pcb *q = RQ2;
    int n = RQ1_n;
    int m = RQ2_n;
    cout << "RQ1的各进程的周转时间为：" << endl;
    while (n > 0)
    {
        cout << p->name << '\t';
        p = p->next;
        n--;
    }
    cout << endl;
    while (n < RQ1_n)
    {
        cout << p->turn << '\t';
        p = p->next;
        n++;
    }
    cout << endl
         << "RQ2的各进程的执行顺序以及周转时间为：" << endl;
    while (m > 0)
    {
        cout << q->name << '\t';
        q = q->next;
        m--;
    }
    cout << endl;
    while (m < RQ2_n)
    {
        cout << q->turn << '\t';
        q = q->next;
        m++;
    }
}
void PCB::Insertsort(tag_pcb *first, int *t)
{
    //从第二个节点开始向后比较
    int j = 1, i = 2;
    tag_pcb *l = first;
    tag_pcb *r = l->next;
    while (i != 5)
    {
        while (j < i)
        {
            if (t[i] < t[j])
            {
                tag_pcb *temp = new tag_pcb;
                temp->next = r->next;
                r->next = temp->next->next;
                temp->next->next = l->next;
                l->next = temp->next;
                delete temp;
                int tm = t[i];
                for (int m = i; m > j; m--)
                    t[m] = t[m - 1];
                t[j] = tm;
                j = 1;
                l = first;
                break;
            }
            else
            {
                j++;
                l = l->next;
            }
        }
        if (j == i)
            r = r->next;
        i++;
    }
    if (t[0] > t[1])
        RQ2 = RQ2->next;
}
void PCB::Charge_RQ1()
{
    tag_pcb *p = RQ1;
    int n = RQ1_n;
    while (n != 0)
    {
        if (p->need > q)
        {
            p->need -= q;
            clock += q;
        }
        else if (p->need <= q && p->need > 0)
        {
            clock += p->need;
            p->need = 0;
            n--;
            p->turn = clock + p->wait;
        }
        p = p->next;
    }
}
void PCB::Charge_RQ2()
{
    tag_pcb *p = RQ2;
    int n = RQ2_n;
    while (n != 0)
    {
        clock += p->need;
        p->turn = p->wait + clock;
        p = p->next;
        n--;
    }
}
int main()
{
    PCB RQ;
    getchar();
    return 0;
}
