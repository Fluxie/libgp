#ifndef TESTOBJECTS_H
#define TESTOBJECTS_H

#include <thread>
#include <vector>

//! Defines types for the tests.
namespace TestObjects
{
    //!
    typedef std::vector< std::thread::id > TestObject;

}

#endif // TESTOBJECTS_H
