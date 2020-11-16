
#ifndef TOP_ELEMS_HEAP_H
#define TOP_ELEMS_HEAP_H

#include <vector>
#include <algorithm>

// TopElemsHeap -- a data structure that keeps only the top maxElems elements
// that are added to it
template<class T>
class TopElemsHeap {
    private:
        // the maximum size of the heap
        const size_t maxElems;
        
        // the elemtns themselves, which are heapified after every operation
        std::vector<T> elems;

    public:
        
        // constructor
        TopElemsHeap(size_t n) : maxElems(n), elems() {}

        // push
        // add a value to the heap; if the heap is already at capacity, the
        // lowest element will be removed by this operaiton
        void push(T& value) {

            // do not even add the element if the heap is at capacity and the 
            // new value would be the smallest entry anyway
            if (this -> elems.size() >= this -> maxElems
                && value <= this -> elems.front()) {
                
                return;
            }
            
            this -> elems.push_back(value);
            std::push_heap(this -> elems.begin(), this -> elems.end(), std::greater());

            // if adding the element breaks the heap capacity, remove the
            // smallest element
            if (this -> elems.size() > this -> maxElems) {
                std::pop_heap(this -> elems.begin(), this -> elems.end(), std::greater());
                this -> elems.pop_back();
            }
        }

        // getElements
        // return an immutable reference to the internal vector
        const std::vector<T>& getElements() const {
            return this -> elems;
        }

        // getMutElements
        // return a mutable reference to the internal vector
        std::vector<T>& getMutElements() {
            return this -> elems;
        }
};

#endif