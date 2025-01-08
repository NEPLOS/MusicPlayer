#pragma once

#include <exception>

template <typename T> 
class Queue
{

    public:

    int size;
    T* arr;
    int front = -1;
    int reel = -1;

    Queue(int size) : size(size)
    {
        arr = new T[size];
    }

    ~Queue()
    {
        delete[] arr;
    }

    void push_back(T item)
    {
        if (is_full())
        {
            std::cout << "Queue is full";
            return;
        }
        if (is_empty())
        {
            front = 0;
        }
        reel = (reel + 1) % size;
        arr[reel] = item;
    }

    T pop_front()
    {
        if (is_empty())
        {
            std::cout << "Queue is empty";
            throw 1;
        }
        T item = arr[front];
        if (front == reel)
        {
            front = -1;
            reel = -1;
        }
        else
        {
            front = (front + 1) % size;
        }
        return item;
    }

    bool is_empty() {return front == -1;}
    bool is_full() {return (reel + 1) % size == front;}

};
