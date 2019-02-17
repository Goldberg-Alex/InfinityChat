//-----------------------------------------
// Written by - 		Alex Goldberg
// File Name-
// Code Reviewer -
// Review Date-
//-----------------------------------------

// this is a template for a cpp program

#include "code_utils.hpp"
#include "wpqueue.hpp"

#include <cassert> //assert()
#include <thread>  //std::thread
//------------------------------------------------------------------------------
using namespace ilrd;
void pop_and_check(WPQueue<int>& queue);

int main()
{
    WPQueue<int> queue;

    // insert and remove
    queue.push(0);
    int a;
    queue.pop(a);

    assert(a == 0);

    for (size_t i = 0; i < 5; i++) {

        std::thread t(&pop_and_check, std::ref(queue));
        t.detach();
    }

    for (size_t i = 0; i < 5; i++) {
        queue.push(3);
    }
}

void pop_and_check(WPQueue<int>& queue)
{
    int i = 0;

    queue.pop(i);
    assert(i == 3);
}

//------------------------------------------------------------------------------
