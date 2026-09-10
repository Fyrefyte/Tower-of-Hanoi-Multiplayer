#include "queue.h"
#include <stdexcept>
#include <iostream>

template <typename T>
Queue<T>::Queue() {}

template <typename T>
const T* Queue<T>::dequeue() {
    if (count-- == 0) throw std::underflow_error("Attempted to dequeue empty stack");
    Node<T>* _out = front;
    front = front->getNext();
    return _out->get();
}

template <typename T>
void Queue<T>::enqueue(T* dataPtr) {
    Node<T> node = Node<T>(dataPtr);
    if (count == 0) {
        std::cout << "Adding first item" << std::endl;
        front = &node;
        back = &node;
    }
    else {
        std::cout << "Adding new item attached to back" << std::endl;
        Node<T>* oldBack = back;
        back = &node;
        oldBack->setNext(&node);
    }
    count++;
}
