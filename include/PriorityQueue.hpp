#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include "Priority.hpp"
#include <queue>

// Implements a queue that operates on the first-in-priority-out principle.
// Objects in the queue are type Priority<T>.
template <class T> class PriorityQueue {
  std::priority_queue<Priority<T>, std::vector<Priority<T>>,
                      std::greater<Priority<T>>>
      queue_;
  unsigned int capacity_;
  unsigned int length_;

public:
  // Default constructor and another constructor.
  PriorityQueue() : queue_(), capacity_(-1), length_(0) {}

  PriorityQueue(unsigned int max) : queue_(), capacity_(max), length_(0) {}

  // There is no destructor because when this goes out of scope it also destroys
  // the queue automatically.

  // Pushes a Priority element into the queue.
  void push(Priority<T> elem) {
    if (length_ < capacity_) {
      queue_.push(elem);
      ++length_;
    }
  }

  // Removes the highest-priority element off of the queue.
  void pop() {
    if (length_ > 0) {
      queue_.pop();
      --length_;
    }
  }

  // Gets the highest-priority element on the queue.
  Priority<T> top() { return queue_.top(); }

  // Returns the max size of the queue, a.k.a. the maximum number of elements.
  unsigned int max_size() const { return capacity_; }

  // Returns the length of the queue.
  unsigned int size() const { return length_; }

  // Returns true if the queue is empty a.k.a. has no elements.
  bool empty() const { return length_ == 0; }
};

#endif