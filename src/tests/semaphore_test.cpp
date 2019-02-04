//-----------------------------------------
// Written by - 		Alex Goldberg
// File Name-
// Code Reviewer -
// Review Date-
//-----------------------------------------

// A testing for for a posix semaphore wrapper


#include "semaphore.hpp"
//------------------------------------------------------------------------------
using namespace ilrd;
int main()
{
	//tests
    Semaphore sem;

    sem.post();
    sem.wait();
    return (0);
}
//------------------------------------------------------------------------------
