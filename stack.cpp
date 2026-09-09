/*
 * Notes
 * =========
 * Weird edge case:
 *  If a Stack of size static_cast<size_t>(-1) is created, everything breaks
 *
 *
 */

#include "stack.h"
#include <stdexcept>

/**
 * @brief Initallizes a new Stack object containing data of type T and with max elements size.
 * @tparam T The type to be stored in the stack.
 * @tparam size The maximum number of elements that can be in the stack simultaneously.
*/
template <typename T, std::size_t size>
Stack<T, size>::Stack() {}

/**
 * @brief Pushes a new value (or values) to the top of the stack.
 * @param value A pointer (or pointers) to the value to be added to the stack. The first value supplied will be added first, leaving the last value on top.
 */
template <typename T, std::size_t size>
template <typename... Ptrs>
void Stack<T, size>::push(Ptrs... value) {
    static_assert((std::is_same_v<Ptrs, T*> && ...), "All arguments to push must be of type T*");
    if (!isEmpty() && !couldAdd(sizeof...(value))) throw std::overflow_error("Stack overflow, attempted to push value to full stack");
    ((data[++_top] = value), ...);
}

/**
 * @brief Gets the top value of the stack.
 * @return A pointer to the value.
 */
template <typename T, std::size_t size>
T* Stack<T, size>::top() {
    if (isEmpty()) throw std::out_of_range("Attempted to get top value of empty stack");
    return data[_top];
}

/**
 * @brief Gets and removes the top value of the stack.
 * @return A pointer to the value.
 */
template <typename T, std::size_t size>
T* Stack<T, size>::pop() {
    if (isEmpty()) throw std::underflow_error("Stack underflow, attempted to pop value from empty stack");
    return data[_top--];
}

template <typename T, std::size_t size>
T* Stack<T, size>::at(size_t index) {
    if (index > _top) throw std::out_of_range("Attempted to get value out of current bounds of stack");
    return data[index];
}

/**
 * @brief Gets the current length of the stack.
 * @return The number of elements currently in the stack.
 */
template <typename T, std::size_t size>
size_t Stack<T, size>::length() {
    return _top + 1;
}

/**
 * @brief Gets the maximum possible length of the stack, as defined at initialization.
 * @return The maximum number of elements that can be in the stack simultaneously.
 */
template <typename T, std::size_t size>
size_t Stack<T, size>::maxLength() {
    return size;
}

/**
 * @brief Clears the stack of all elements.
 */
template <typename T, std::size_t size>
void Stack<T, size>::empty() {
    _top = -1; // TODO figure out what it means by needing to delete stuff here
}

/**
 * @brief Checks if the stack is empty.
 * @return Whether or not the stack is empty.
 */
template <typename T, std::size_t size>
bool Stack<T, size>::isEmpty() {
    return _top == -1;
}

/**
 * @brief Checks if the stack is full.
 * @return Whether or not the stack is full.
 */
template <typename T, std::size_t size>
bool Stack<T, size>::isFull() {
    return _top == size - 1;
}

/**
 * @brief Checks whether the stack has space to add X more items.
 * @param The amount of items you wish to add.
 * @return Whether or not the stack has the specified capacity.
 */
template <typename T, std::size_t size>
bool Stack<T, size>::couldAdd(size_t more) {
    return _top < size - more;
}

//////////////////////////////////////////////////////////////

template <typename T1, typename T2, std::size_t size>
StackPaired<T1, T2, size>::StackPaired() {}

template <typename T1, typename T2, std::size_t size>
void StackPaired<T1, T2, size>::push(T1* value1, T2* value2) {
    stack1.push(value1);
    stack2.push(value2);
}

template <typename T1, typename T2, std::size_t size>
void StackPaired<T1, T2, size>::top(T1* value1, T2* value2) {
    value1 = stack1.top();
    value2 = stack2.top();
}

template <typename T1, typename T2, std::size_t size>
T1* StackPaired<T1, T2, size>::top1() {
    return stack1.top();
}

template <typename T1, typename T2, std::size_t size>
T2* StackPaired<T1, T2, size>::top2() {
    return stack2.top();
}

template <typename T1, typename T2, std::size_t size>
void StackPaired<T1, T2, size>::pop(T1* value1, T2* value2) {
    value1 = stack1.pop();
    value2 = stack2.pop();
}

template <typename T1, typename T2, std::size_t size>
T1* StackPaired<T1, T2, size>::pop1() {
    stack2.pop();
    return stack1.pop();
}

template <typename T1, typename T2, std::size_t size>
T2* StackPaired<T1, T2, size>::pop2() {
    stack1.pop();
    return stack2.pop();
}

template <typename T1, typename T2, std::size_t size>
void StackPaired<T1, T2, size>::at(T1* value1, T2* value2, size_t index) {
    value1 = stack1.at(index);
    value2 = stack2.at(index);
}

template <typename T1, typename T2, std::size_t size>
T1* StackPaired<T1, T2, size>::at1(size_t index) {
    return stack1.at(index);
}

template <typename T1, typename T2, std::size_t size>
T2* StackPaired<T1, T2, size>::at2(size_t index) {
    return stack2.at(index);
}

template <typename T1, typename T2, std::size_t size>
size_t StackPaired<T1, T2, size>::length() {
    return stack1.length();
}

template <typename T1, typename T2, std::size_t size>
size_t StackPaired<T1, T2, size>::maxLength() {
    return size;
}

template <typename T1, typename T2, std::size_t size>
void StackPaired<T1, T2, size>::empty() {
    stack1.empty();
    stack2.empty();
}

template <typename T1, typename T2, std::size_t size>
bool StackPaired<T1, T2, size>::isEmpty() {
    return stack1.isEmpty();
}

template <typename T1, typename T2, std::size_t size>
bool StackPaired<T1, T2, size>::isFull() {
    return stack1.isFull();
}

template <typename T1, typename T2, std::size_t size>
bool StackPaired<T1, T2, size>::couldAdd(size_t more) {
    return stack1.couldAdd(more);
}
