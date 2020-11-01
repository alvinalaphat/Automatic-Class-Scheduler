#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include "Priority.hpp"
#include <queue>

// Implements a queue that operates on the first-in-priority-out principle.
// Objects in the queue are type Priority<T>.
template <class T>
class PriorityQueue {

    std::priority_queue<Priority<T>, std::vector<Priority<T>>, std::greater<Priority<T>>> mQueue;
    unsigned int mCapacity;
    unsigned int mLength;

public:
    // Default constructor and another constructor.
    PriorityQueue()
        : mQueue()
        , mCapacity(-1)
        , mLength(0)
    {
    }

    PriorityQueue(unsigned int max)
        : mQueue()
        , mCapacity(max)
        , mLength(0)
    {
    }

    // There is no destructor because when this goes out of scope it also destroys the queue automatically.

    // Pushes a Priority element into the queue.
    void push(Priority<T> elem)
    {
        if (mLength < mCapacity) {
            mQueue.push(elem);
            ++mLength;
        }
    }

    // Removes the highest-priority element off of the queue.
    void pop()
    {
        if (mLength > 0) {
            mQueue.pop();
            --mLength;
        }
    }

    // Gets the highest-priority element on the queue.
    Priority<T> top()
    {
        return mQueue.top();
    }

    // Returns the max size of the queue, a.k.a. the maximum number of elements.
    unsigned int max_size() const
    {
        return mCapacity;
    }

    // Returns the length of the queue.
    unsigned int size() const
    {
        return mLength;
    }

    // Returns true if the queue is empty a.k.a. has no elements.
    bool empty() const
    {
        return mLength == 0;
    }
};

#endif