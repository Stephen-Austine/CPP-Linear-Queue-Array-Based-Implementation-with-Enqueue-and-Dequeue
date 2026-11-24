/*
 * linear_queue.cpp
 * ----------------------------------------------------------------------------
 * Array-based LINEAR queue (FIFO) in C++.
 *
 *   - Fixed capacity of QUEUE_CAPACITY integers, stored in a plain int array.
 *   - addQ(item)  : enqueue at the rear   (prints "queue is full" on overflow)
 *   - delQ()      : dequeue from the front (prints "queue is empty" and
 *                   returns 0 on underflow)
 *
 * Note: both indices only ever move forward, so space released by dequeued
 * items is never reused. See README.md ("Known limitations") and
 * circular_queue.cpp for the fix.
 *
 * Build : g++ -std=c++17 -Wall -Wextra -pedantic -o bin/linear_queue src/linear_queue.cpp
 * Run   : ./bin/linear_queue
 * ----------------------------------------------------------------------------
 */
#include <iostream>
using namespace std;

// Renamed from `size` (original) to avoid an "ambiguous reference" compile
// error with std::size under C++17 and later.
const int QUEUE_CAPACITY = 5;

class queue
{
	int front , rear;
	int a[QUEUE_CAPACITY];
	public:
	queue(){front=0;rear=0;} /*Constructor to create an empty queue*/
	void addQ(int item)/* enqueue operation*/
	{
		if(rear==QUEUE_CAPACITY)
			cout<<"queue is full"<<endl;
 		else
			a[rear++]=item;
	}
	int delQ() /* dequeue operation*/
	{
		if(front==rear)
			{
				cout<<"queue is empty"<<endl;
				return 0;
			}
		else
			return a[front++];
	}
};

int main()
{
	/*enqueue operation*/
	queue q1;
	q1.addQ(30);
	q1.addQ(50);
	q1.addQ(90);

	/*dequeue operations*/
	cout<<q1.delQ()<<endl;
	cout<<q1.delQ()<<endl;
	cout<<q1.delQ()<<endl;
	cout<<q1.delQ()<<endl;

	return 0;
}
