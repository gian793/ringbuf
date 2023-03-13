
# Ring Buffer

This repository contains the implementation of a generic ring buffer, designed to handle elements of different size.

## How to use it

Import `ringbuf.cpp` in your project with the related include file i.e. `ringbuf.hpp`.

In the application the user has first to declare (statically or dynamically) the memory pool intended to be used as ring buffer.\
At this point the ring buffer constructor can be called to instantiate a ring buffer object with the constructor:

- `ringbuf( ... )`

In order to push new items in front of the ring buffer use:

- `push()`

This ring buffer implementation permits only sequential access i.e. from the head or from the tail, using the following methods:

- `getHead()`
- `getTail()`
  
Iteration over the items of the buffer can be done by using items' pointers:

- `pxNext`
- `pxPrev`

**Note:** every new item pushed in the ring buffer requires some additional space of the memory for keeping track of\
next and previous items and the size of the item itself. This overhead depends on the compiler and on the target.\
On a 32-bit target compiled with gcc, the overhead is 12 bytes.

## Reference example

In the following example, a ring buffer is created with a memory pool of size 1024 [byte].\
Three arrays of different size are pushed into the ring buffer.\
Eventually, the second array pushed in (item2) is read back and a test on the second element of the array executed.

```c
#include <iostream>
#include <cstring>

#include "ringbuf.cpp"

int main()
{
    constexpr size_t  mem_pool_size = 1024U;

    /* Memory pool. */
    uint8_t memPool[ mem_pool_size  ];

    /* Create the ring buffer. */
    ringbuf  myFirstRingBuf( memPool, mem_pool_size );

    /* Items to push into the ring buffer. */
    uint16_t item1[ ] = { 1, 2, 3, 4, 5 };
    uint8_t  item2[ ] = { 6, 7, 8 };
    uint32_t item3[ ] = { 21, 22, 23, 24 };

    /* Push  items in. */
    myFirstRingBuf.push( item1, sizeof( item1 ) );
    myFirstRingBuf.push( item2, sizeof( item2 ) );
    myFirstRingBuf.push( item3, sizeof( item3 ) );

    /* Get pointer to the second item pushed (Notice: getTail->pxNext). */
    const rbItem_t* pItem2 = myFirstRingBuf.getTail()->pxNext;

    /* Buffer to read back the 2nd item. */
    uint8_t rxBuf[ sizeof( item2 ) ] = {0};

        /* Get data from 2nd item. */
    myFirstRingBuf.getData( pItem2, rxBuf );;

    /* Test 1 element read. */
    if( rxBuf[1] == 7 )
    {
        std:cout << "ok" << std::endl;
    }
    else
    {
        std:cout << "NOK" << std::endl;
    }

    return 0;
}