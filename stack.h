#ifndef STACK_H
#define STACK_H

#include <cstddef>
#include <iostream>
#include <sstream>

template <typename T, std::size_t size>
class Stack
{
private:
    T* data[size];
    size_t _top = -1;
public:
    Stack();
    template <typename... Ptrs> void push(Ptrs...);
    T* top();
    T* pop();
    T* at(size_t);
    size_t length();
    size_t maxLength();
    void empty();
    bool isFull();
    bool isEmpty();
    bool couldAdd(size_t);

    /**
     * @brief Retrieves the private data array of the given stack for use.
     * @param stack The stack to retrieve from.
     * @return A pointer to the first element of the array.
     */
    friend T** getDataArray(const Stack<T, size>& stack) {
        return stack.data;
    }

    /**
     * @brief Prints the private data array.
     * @param stack The stack to retrieve from.
     * @param topToBottom Whether to print top to bottom or bottom to top.
     * @param printAboveCurrentMax Whether to print values above the current max value (not recommended)
     */
    friend void printDataArray(const Stack<T, size>& stack, bool topToBottom = true, bool printAboveCurrentMax = false) {
        std::ostringstream stream;
        stream << "Stack contents (" << (topToBottom ? "top to bottom" : "bottom to top") << "): ";
        if (topToBottom) {
            if (printAboveCurrentMax) {
                stream << stack.data[stack.maxLength() - 1];
                for (size_t i = stack.maxLength() - 2; i >= 0; i--) {
                    stream << ", " << stack.data[i];
                }
            }
            else {
                stream << stack.data[stack.length() - 1];
                for (size_t i = stack.length() - 2; i >= 0; i--) {
                    stream << ", " << stack.data[i];
                }
            }
        }
        else {
            if (printAboveCurrentMax) {
                stream << stack.data[0];
                for (size_t i = 1; i < stack.maxLength(); i++) {
                    stream << ", " << stack.data[i];
                }
            }
            else {
                stream << stack.data[0];
                for (size_t i = 1; i < stack.length(); i++) {
                    stream << ", " << stack.data[i];
                }
            }
        }
        stream << "]";

        std::cout << stream.str();
    }
};

#endif // STACK_H
