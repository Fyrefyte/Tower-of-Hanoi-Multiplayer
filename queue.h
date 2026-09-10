#pragma once

#ifndef QUEUE_H
#define QUEUE_H

#include <cstdlib>

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
    Queue();
    void enqueue(T* data);
    const T* dequeue();
    size_t length() { return count; };
};

// TODO make this work
template <typename T>
class QueueIter : Queue<T>
{
private:
    Node<T>* iter = nullptr;
};

#endif // QUEUE_H
