
#pragma once

#include <iostream>

template <class T>
class Stack
{

public:
    int size;
    T* arr;
    int top = -1;

    Stack(int size) : size(size)
    {
        arr = new T[size];
    }

    ~Stack()
    {
        delete[] arr;
    }

    void push_back(T item)
    {
        if (is_full())
        {
            std::cout << "stack is full \n";
            return;
        }
        arr[++top] = item;
    }

    T pop_back()
    {
        if (is_empty())
        {
            std::cout << "stack is empty \n";
            throw 2;
        }
        return arr[top--];
    }

    bool is_full() { return top == size - 1; }
    bool is_empty() { return top == -1; }

};
