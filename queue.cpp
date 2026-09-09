#include "queue.h"
#include <stdexcept>

template <typename T>
Queue<T>::Queue() {}

template <typename T>
T* Queue<T>::dequeue() {
    if (count-- == 0) throw std::underflow_error("Attempted to dequeue empty stack");
    T* _out = front;
    front = front->getNext();
    return _out;
}

template <typename T>
void Queue<T>::enqueue(T* dataPtr) {
    back->setNext(Node<T>(dataPtr));
    count++;
}
