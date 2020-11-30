#include <iostream>
#include <unordered_map>
using namespace std;

const int ap = 5;
const int pp = 3;
const int total_instruction = 10;

int page[ap] = { 0 };
int pagecontrol[pp] = { 0 };
int main_p[total_instruction] = { 1, 3, 2, 4, 1, 0, 1, 3, 4, 0 };
int diseffect = 0;

class Node
{
public:
	int key, val;
	Node* next = nullptr;
	Node* prev = nullptr;
	Node()
	{
		key = 0;
		val = 0;
	};
	Node(int k, int v)
	{
		this->key = k;
		this->val = v;
	}
};

class LinkedHashMap
{
public:
	int cap;
	Node* start = new Node();
	Node* end = new Node();
	unordered_map<int, Node*> map;
	LinkedHashMap(int size)
	{
		this->cap = size;
		start->next = end;
		end->prev = start;
	};
	~LinkedHashMap() {
		Node* t = start;
		Node* p;
		do{
			p = t;
			t = t->next;
			delete p;
		}while (t != nullptr);
	};
	// 链表的操作
	Node* push_back(int key, int val)
	{
		Node* t = new Node(key, val);
		Node* p = end->prev;
		end->prev = t;
		t->next = end;

		p->next = t;
		t->prev = p;
		return t;
	}
	void delNode(Node* t)
	{
		if (t == end || t == start)
		{
			return;
		}
		Node* p = t->prev;
		Node* n = t->next;
		p->next = n;
		n->prev = p;
		delete t;
	}
	void mov2end(Node* t)
	{
		if (t == end || t == start)
		{
			return;
		}
		t->prev->next = t->next;
		t->next->prev = t->prev;
		Node* p = end->prev;
		end->prev = t;
		t->next = end;

		p->next = t;
		t->prev = p;
		
		// 不能这么写
		/*delNode(t);
		push_back(t->key, t->val);*/
	}
	// LinkedHashMap
	Node* search(int key)
	{
		unordered_map<int, Node*>::iterator it = map.find(key);
		if (it == map.end())
		{
			return nullptr;
		}
		else
		{
			return it->second;
		}
	}
	void delNode(int key)
	{
		unordered_map<int, Node*>::iterator it = map.find(key);
		Node* t = it->second;
		map.erase(it);
		delNode(t);
	}
	void removeLeastRecently()
	{
		Node* t = start->next;
		int k = t->key;
		delNode(k);
	}
	void addRecently(int key, int val)
	{
		Node* t = push_back(key, val);
		map.insert(pair<int, Node*>(key, t));
	}
	void makeRecently(int key)
	{
		Node* t = search(key);
		mov2end(t);
	}
	void put(int key, int val)
	{
		if (search(key) != nullptr)
		{
			makeRecently(key);
		}
		else
		{
			if ((int)map.size() == cap)
			{
				removeLeastRecently();
			}
			addRecently(key, val);
		}
	}
	bool isEmpty() {
		return (int)map.size() < cap;
	}
	int front() {
		return end->prev->key;
	}
	int endKey() {
		return start->next->key;
	}
};


int main()
{
	LinkedHashMap HM(pp);
	for (int i = 0; i < ap; i++)
	{
		page[i] = -1;
	}
	for (int i = 0; i < pp; i++)
	{
		pagecontrol[i] = -1;
	}
	for (auto i : main_p)
	{
		cout << "page:" << i << ",State:" << page[i] << endl;
		if (page[i] == -1)
		{
			// 未命中
			diseffect++;
			cout << "diseffect:" << diseffect << endl;
			if (!HM.isEmpty())
			{
				for (auto j : page) {
					cout << j << '\t';
				}
				cout << endl;
				for (auto j : pagecontrol) {
					cout << j << '\t';
				}
				cout << endl;
				int page_po = HM.endKey();
				int pagecontrol_po = page[page_po];
				cout << "page was replaced:" << page_po << endl;
				page[page_po] = -1;

				page[i] = pagecontrol_po;
				pagecontrol[pagecontrol_po] = i;
				HM.put(i, pagecontrol_po);
			}
			else
			{
				int position;
				for (int j = 0; j < pp; j++)
				{
					if (pagecontrol[j] == -1)
					{
						position = j;
						break;
					}
				}
				page[i] = position;
				pagecontrol[position] = i;
				HM.put(i, position);
			}
		}
		else
		{
			// 命中
			HM.makeRecently(i);
		}
		// cout << endl;
		cout << endl;
	}
	cout << 1 - diseffect * 1.0 / total_instruction << endl;
	return 0;
}