
#ifndef TOP_ELEMS_HEAP_H
#define TOP_ELEMS_HEAP_H

#include <vector>
#include <algorithm>

// TopElemsHeap -- a data structure that keeps only the top maxElems elements
// that are added to it
template<class T>
class TopElemsHeap {
    private:
        const size_t maxElems;
        
        std::vector<T> elems;

    public:
        
        TopElemsHeap(size_t n) : maxElems(n), elems() {}

        void push(T value) {

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

        const std::vector<T>& getElements() const {
            return this -> elems;
        }
};

#endif