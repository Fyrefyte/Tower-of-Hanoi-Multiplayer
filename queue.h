#pragma once

#ifndef QUEUE_H
#define QUEUE_H

#include <cstdlib>
#include <stdexcept>

template <typename T>
class Node
{
private:
    const T* data;
    Node<T>* next;
public:
    Node(T* dataPtr, Node<T>* nextPtr = nullptr) : data(dataPtr) { next = nextPtr; }
    const T* get() { return data; }
    Node<T>* getNext() { return next; }
    void setNext(Node<T>* node) { next = node; }
};

// Node(data) next--> Node(data) next--> Node(data) next--> nullptr
//  ^ front                               ^ back
template <typename T>
class Queue
{
private:
    Node<T>* front = nullptr;
    Node<T>* back = nullptr;
    size_t count = 0;
public:
    Queue() {}
    void enqueue(T*);
    const T* dequeue();
    size_t length() { return count; }
};

template <template <typename> class Container, typename T>
class ContainerQueue
{
private:
    Node<Container<T>>* front = nullptr;
    Node<Container<T>>* back = nullptr;
    size_t count = 0;
public:
    ContainerQueue() {}
    void enqueue(Container<T>*);
    const Container<T>* dequeue();
    size_t length() { return count; }
};

template <template <typename> class Container, typename T>
const Container<T>* ContainerQueue<Container, T>::dequeue() {
    if (count == 0) throw std::underflow_error("Attempted to dequeue empty queue");
    Node<Container<T>>* _out = front;
    front = front->getNext();
    const Container<T>* _result = _out->get();
    delete _out;
    --count;
    return _result;
}

template <template <typename> class Container, typename T>
void ContainerQueue<Container, T>::enqueue(Container<T>* dataPtr) {
    Node<Container<T>>* node = new Node<Container<T>>(dataPtr);
    if (count == 0) {
        front = node;
        back = node;
    }
    else {
        Node<Container<T>>* oldBack = back;
        back = node;
        oldBack->setNext(node);
    }
    count++;
}

// TODO make this work
template <typename T>
class QueueIter : Queue<T>
{
private:
    Node<T>* iter = nullptr;
};

#endif // QUEUE_H
