#include "queue.h"
#include <stdexcept>

/**
 * @brief Gets and returns the value at the front of the queue.
 * @return A pointer to the value.
 */
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

/**
 * @brief Adds a value to the back of the queue.
 * @param dataPtr The pointer to add.
 */
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

/**
 * @brief Clears the queue.
 */
template <typename T>
void Queue<T>::clear() {
    while (count > 0) dequeue();
}
