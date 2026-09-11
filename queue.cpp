#include "queue.h"
#include <stdexcept>

template <typename T>
const T* Queue<T>::dequeue() {
    if (count == 0) throw std::underflow_error("Attempted to dequeue empty queue");
    Node<T>* _out = front;
    front = front->getNext();
    const T* _result = _out->get();
    delete _out;
    --count;
    return _result;
}

template <typename T>
void Queue<T>::enqueue(T* dataPtr) {
    Node<T>* node = new Node<T>(dataPtr);
    if (count == 0) {
        front = node;
        back = node;
    }
    else {
        Node<T>* oldBack = back;
        back = node;
        oldBack->setNext(node);
    }
    count++;
}
