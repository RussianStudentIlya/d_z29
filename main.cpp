#include <iostream>
#include <mutex>
using namespace std;

struct Node
{
	int _value;
	Node* _next;
	mutex* _node_mutex;

    Node(int value, Node* next = nullptr) : _value(value), _next(next)
    {};
};

class FineGrainedQueue
{
	Node* head;
	mutex* queue_mutex;
public:
	void insertIntoMiddle(int value, int pos);
};

int main()
{
	return 0;
}

void FineGrainedQueue::insertIntoMiddle(int value, int pos)
{
    Node* prev, * cur; // указатели на предыдущий и текущий элемент

    Node* _newNode = new Node(value);

    //queue_mutex->lock(); // теперь наш head нельзя будет поменять

    prev = this->head; // старт: предыдущий = начало списка
    cur = this->head->_next; // старт: текущий = второй элемент списка

    int index = 0;

    //Поиск нужной позиции или конца списка
    while (cur != nullptr && index < pos)
    {
        Node* old_prev = prev;
        prev = cur;
        cur = cur->_next;
        index++;
    }

    lock_guard<mutex> prevLock(*prev->_node_mutex); //блокировка мьютекса предыдущего узла

    if (cur == nullptr)
    {
        prev->_next = _newNode; //добавляем в конец списка
    }
    else
    {
        lock_guard<mutex> curLock(*cur->_node_mutex); //блокировка мьютекса текущего узла

        _newNode->_next = cur;
        prev->_next = _newNode; //добавляем в нужную позицию
    }
}
