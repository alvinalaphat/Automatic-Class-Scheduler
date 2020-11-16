
#ifndef SHARED_VECTOR_H
#define SHARED_VECTOR_H

#include <vector>
#include <string.h>
#include <iostream>

// SharedVector
// A specialized vector class that only allows items to be appended not removed,
// and which implements a copy on write mechanism to reduce data use; it also
// allows for one value at a time to be queued before being actually added
// to the vector
template <class T>
class SharedVector {
    private:
        
        // An underlying reference couted vector which the SharedVector retains
        // a reference to
        struct Vector {
            T * data;
            size_t cap;
            size_t size;
            size_t refCount;

            // Constructor for empty Vector
            Vector() :
                data(new T[1]),
                cap(1),
                size(0),
                refCount(1) {}

            // Copy constructor
            Vector(Vector& vec) :
                refCount(1),
                data(new T[vec.cap]),
                cap(vec.cap),
                size(vec.size) {
                
                memcpy(this -> data, vec.data, sizeof(T) * vec.size);
            }

            // Constructor which copies only the first n elements from another
            // Vector
            Vector(Vector& vec, size_t nElems) :
                data(new T[vec.cap]),
                cap(vec.cap),
                size(nElems),
                refCount(1) {
                
                memcpy(this -> data, vec.data, sizeof(T) * nElems);
            }

            // Copy assignment operator
            Vector& operator=(const Vector& rhs) {
                if (&rhs != this) {
                    delete[] this -> data;
                    
                    this -> data = new T[vec.cap];
                    this -> cap = rhs.cap;
                    this -> size = rhs.size;
                    this -> refCount = 1;
                    memcpy(this -> data, rhs.data, sizeof(T) * rhs.size);
                }

                return *this;
            }

            // push
            // add an element to the end of the Vector, resize the internal
            // buffer if necessary
            void push(T& value) {
                if (this -> cap <= this -> size) {
                    T * newData = new T[this -> cap * 2];
                    memcpy(newData, this -> data, sizeof(T) * this -> size);
                    delete[] this -> data;
                    this -> data = newData;
                    this -> cap *= 2;
                }

                this -> data[this -> size] = value;
                this -> size += 1;
            }

            ~Vector() {
                delete[] this -> data;
            }
        };
    
        // internal count of how many elements are being used; this may be less
        // than are stored in the underlying vector
        size_t size;

        // reference to the underlying vector
        Vector * vec;

        // the queued element and whether an element is currently queued
        T queued;
        bool queueFull;

    public:

        // Constructor for empty SharedVector
        SharedVector() : size(0), vec(new Vector()), queued(),
            queueFull(false) {}

        // Copy constructor: increases underlying vector's reference count
        // instead of copying
        SharedVector(const SharedVector& rhs) : size(rhs.size), vec(rhs.vec),
            queued(rhs.queued), queueFull(rhs.queueFull) {
            
            this -> vec -> refCount += 1;
        }

        // Copy assignment: increases underlying vector's reference count
        // instead of copying
        SharedVector& operator=(const SharedVector& rhs) {
            if (&rhs != this) {
                this -> vec -> refCount -= 1;
                if (this -> vec -> refCount == 0) {
                    delete vec;
                }

                this -> vec = rhs.vec;
                this -> vec -> refCount += 1;
                this -> size = rhs.size;
                this -> queued = rhs.queued;
                this -> queueFull = rhs.queueFull;
            }
            return *this;
        }

        // destructor: only frees underlying vector if reference count is zero
        ~SharedVector() {
            this -> vec -> refCount -= 1;
            if (this -> vec -> refCount == 0) {
                delete vec;
            }
        }

        // queue
        // queue an element to be added, but do not actually add it yet
        void queue(T& value) {
            this -> queued = value;
            this -> queueFull = true;
        }

        // flushQueue
        // add a queued element (if any) to the underlying vector
        void flushQueue() {

            // check if there is a queued element
            if (this -> queueFull) {

                // copy the underlying vector if another reference has already
                // used more elements
                if (this -> size < this -> vec -> size) {
                    Vector * oldVec = this -> vec;
                    this -> vec = new Vector(*oldVec, this -> size);
                    
                    // free the underlying vector if it is unused
                    oldVec -> refCount -= 1;
                    if (oldVec -> refCount == 0) {
                        delete oldVec;
                    }
                }

                // append the element
                this -> vec -> push(this -> queued);
                this -> size += 1;
                this -> queueFull = false;
            }
        }

        // getArray
        // get an immutable pointer to the data
        const T * getArray() const {
            return this -> vec -> data;
        }

        // getSize
        // get the size number of elements in the SharedVector
        size_t getSize() const {
            return this -> size;
        }

        // display
        // display the vector contents to stdout
        void display() const {

            // display the reference count and reference
            std::cout << "(" << this -> vec -> refCount << " @"
                << (void*)this -> vec -> data << ")";
            
            // display the elements in the vector
            std::cout << " [";
            for (size_t i = 0; i < this -> size; ++i) {
                std::cout << this -> vec -> data[i] << ", ";
            }
            std::cout << "]" << std::endl;
        }
};

#endif