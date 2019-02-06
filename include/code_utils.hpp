// ****************************************
// * Written by - Alex Goldberg              *
// * File Name-    code_utils.h              *
// *                                         *
// *****************************************

// this is a collection of utility macros for c++

// VERSION 1.0

// Change Log
// -------------------------

// 1.0: -created file

#ifndef HRD6_CODE_UTILS
#define HRD6_CODE_UTILS

#include <cassert>  //assert()
#include <cstdlib>  //EXIT_SUCCESS, EXIT_FAILURE
#include <cstring>  //strcmp
#include <iostream> //std::cout, std::endl

#if __cplusplus < 201103L
#define nullptr 0
#endif //__cplusplus < 201103L

namespace ilrd {

template <typename T>
static T Max(T t1, T t2)
{
    return (t1 > t2 ? t1 : t2);
}

template <typename T>
static T Min(T t1, T t2)
{
    return (t1 < t2 ? t1 : t2);
}

const size_t WORD_SIZE = (sizeof(void*));

// #define NUM_ELEMENTS(x) (sizeof((x)) / sizeof((x)[0]))
// use the Array class instead

template <typename T>
static T Abs(T t)
{
    return ((t > 0) * t + (t < 0) * (-t));
}

template <typename T>
static void print_result(T result,
                         T expected,
                         std::string status,
                         std::string filename,
                         int line,
                         const std::string message)
{
    std::cout << status << " received: " << result
              << " expected: " << expected << " ["
              << filename.substr(filename.find_last_of('/') + 1) << ":" << line
              << "] - " << message << std ::endl;
}

template <typename T>
static void check_equal(T result,
                        T expected,
                        int line,
                        std::string filename,
                        bool verbose = false,
                        std::string message = "")
{
    if (result == expected) {

        if (verbose) {
            print_result(result,
                         expected,
                         "SUCCESS - equal - ",
                         filename,
                         line,
                         message);
        }

    } else {
        print_result(
            result, expected, "FAILURE - equal - ", filename, line, message);
    }
}

template <typename T>
static void check_different(T result,
                            T expected,
                            int line,
                            bool verbose = false,
                            std::string message = "")
{
    if (result != expected) {

        if (verbose) {
            print_result(result, expected, "SUCCESS - diff - ", line, message);
        }

    } else {
        print_result(result, expected, "FAILURE - diff - ", line, message);
    }
}

#define CHECK_EQUAL(result, expected, verbose, message)                        \
    check_equal((result),                                                      \
                (expected),                                                    \
                __LINE__,                                                      \
                __FILE__,                                                      \
                (verbose),                                                     \
                std::string(message));

} // namespace ilrd
#endif // HRD6_CODE_UTILS
