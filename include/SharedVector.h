
#ifndef SHARED_VECTOR_H
#define SHARED_VECTOR_H

#include <vector>
#include <string.h>
#include <iostream>

template <class T>
class SharedVector {
    private:
        
        struct Vector {
            T * data;
            size_t cap;
            size_t size;
            size_t refCount;

            Vector() :
                data(new T[1]),
                cap(1),
                size(0),
                refCount(1) {}

            Vector(Vector& vec) :
                refCount(1),
                data(new T[vec.cap]),
                cap(vec.cap),
                size(vec.size) {
                
                memcpy(this -> data, vec.data, sizeof(T) * vec.size);
            }

            Vector(Vector& vec, size_t nElems) :
                data(new T[vec.cap]),
                cap(vec.cap),
                size(nElems),
                refCount(1) {
                memcpy(this -> data, vec.data, sizeof(T) * nElems);
            }

            Vector& operator=(const Vector& rhs) {
                this -> data = new T[vec.cap];
                this -> cap = rhs.cap;
                this -> size = rhs.size;
                this -> refCount = 1;
                memcpy(this -> data, rhs.data, sizeof(T) * rhs.size);

                return *this;
            }

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
    
        size_t size;
        Vector * vec;
        T queued;
        bool queueFull;

    public:

        SharedVector() : size(0), vec(new Vector()), queued(),
            queueFull(false) {}

        SharedVector(const SharedVector& rhs) : size(rhs.size), vec(rhs.vec),
            queued(rhs.queued), queueFull(rhs.queueFull) {
            
            this -> vec -> refCount += 1;
        }

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

        ~SharedVector() {
            this -> vec -> refCount -= 1;
            if (this -> vec -> refCount == 0) {
                delete vec;
            }
        }

        void queue(T& value) {
            this -> queued = value;
            this -> queueFull = true;
        }

        void flushQueue() {
            if (this -> queueFull) {
                if (this -> size < this -> vec -> size) {
                    Vector * oldVec = this -> vec;
                    this -> vec = new Vector(*oldVec, this -> size);
                    
                    oldVec -> refCount -= 1;
                    if (oldVec -> refCount == 0) {
                        delete oldVec;
                    }
                }

                this -> vec -> push(this -> queued);
                this -> size += 1;
                this -> queueFull = false;
            }
        }

        const T * getArray() const {
            return this -> vec -> data;
        }

        size_t getSize() const {
            return this -> size;
        }

        void display() const {

            std::cout << "(" << this -> vec -> refCount << " @"
                << (void*)this -> vec -> data << ")";
            std::cout << " [";
            for (size_t i = 0; i < this -> size; ++i) {
                std::cout << this -> vec -> data[i] << ", ";
            }
            std::cout << "]" << std::endl;
        }
};

#endif