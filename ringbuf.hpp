/**
 * \file            ringbuf.hpp
 * \brief           Ring buffer to store data of different size. 
 */

/*
 * Copyright (C) 2021 Giancarlo Marcolin
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * This file is part of ringbuf - Ring buffer for variable size elements.
 *
 * Author:          Giancarlo Marcolin <giancarlo.marcolin@gmail.com>
 * Version:         v1.0.0
 */

#ifndef C_RING_BUF_HPP
#define C_RING_BUF_HPP


#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup ringbuf_struct_types
 * @brief Struct holding pointers and data size.
 */

struct rbItem {
    struct rbItem *pxNext;  /**< Pointer to next element of the buffer. */
    struct rbItem *pxPrev;  /**< Pointer to previsous element of the buffer. */
    std::size_t xItemSize;
  };

typedef struct rbItem rbItem_t;

/**
 * @class ringBuf 
 *
 * @brief Class that provides the standard ringBuf interface.
 *
 * @note This class is intended to be instantiated by the user.  Use
 * ringBuf::ringBuf
 *
 */

class ringbuf {

  private:
    std::uint8_t* pcBuf;          /**< Pointer to the memory where the ringBuf instance is implemented. */
    const std::size_t xBufSize;   /**< Size of the momory in [byte]. */
    std::size_t  xTotItemCnt;     /**< Number of element present in the buffer. */

    rbItem_t* pxHead;             /**< Head of the buffer i.e. last element inserted. */
    rbItem_t* pxTail;             /**< Tail of the buffer i.e. oldest element retained. */

    /* Private methods. */
    void reset( void );
    std::uint8_t* getNextPtr( const std::size_t xItemSize );
    void pushItem( const void* pxHeader, const void* pxItem, const std::size_t xItemSize );

  public:

    ringbuf( std::uint8_t* pcPool, const std::size_t xPoolSize );

    //~ringbuf() {};

    void flush( void );

    bool push( const void* pxItem, const size_t xItemSize );

    bool isEmpty( void );

    bool deleteHead( void );

    bool deleteTail( void );

    bool getData( const rbItem_t* pxItem, std::uint8_t* pcDstBuf );

    const std::size_t getHeadSize( void );

    const std::size_t getTailSize( void );

    const std::size_t getItemsCnt( void );

    const rbItem_t* getHead( void );

    const rbItem_t* getTail( void );
};


#ifdef __cplusplus
}
#endif

#endif //C_RING_BUF_HPP
