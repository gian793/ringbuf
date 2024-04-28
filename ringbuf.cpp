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


/* Standard includes. */
#include <cstring>

/* Include API header. */
#include "ringbuf.hpp"

/*-----------------------------------------------------------*/

/**
 * @brief Main to compile under visual studio code.
 *
 * @note Define the symbol VISUALS>TUDIO_CODE to enable its compilation.
 */

// #ifdef VISUAL_STUDIO_CODE
// #include <iostream>
// #include <vector>
// int main() {
//     std::vector<std::string> msg {"Hello ringBuffer!"};

//     for (const std::string& word : msg)
//     {
//         std::cout << word << " ";
//     }
    
//     std::cout << std::endl;
// }
// #endif

/*--------------------- Private methods ---------------------*/

/**
 * @brief Resets the ring buffer.
 * 
 * @note Private method.
 * 
 */

void ringbuf::reset( void ) 
{
    /* pHead and pTail init. */
    pxHead = ( rbItem_t* )pcBuf;

    pxHead->pxNext = pxHead;
    pxHead->pxPrev = pxHead;
    pxHead->xItemSize = 0;

    xTotItemCnt = 0;

    pxTail = pxHead;
}

/**
 * @brief Identifies the position in the ring buffer
 *        where the new item header shall be inserted.
 * 
 * @note Private method.
 * 
 * @param[in] xItemSize Size [byte] of the item to insert.
 * @param[out] Pointer to the postion.
 *
 */

std::uint8_t* ringbuf::getNextPtr( const std::size_t xItemSize ) 
{
    std::size_t topFreeSpace = 0;
    std::size_t bottomFreeSpace = 0;
    std::uint8_t* ptrNext = nullptr;
    std::uint8_t* pHeader = nullptr;

    if ( xTotItemCnt == 0 )
    {
        /* Empty buffer. */
        pHeader = pcBuf;
        topFreeSpace = xBufSize;
    }
    else
    {
        /* Determine position of next item. */
        ptrNext = ( std::uint8_t* )pxHead + sizeof( rbItem_t ) + pxHead->xItemSize;
   
        /* Buffer roll-over check. */
        if( ptrNext > &pcBuf[ xBufSize - 1 ] )
        {
            ptrNext = pcBuf + ( ptrNext - &pcBuf[ xBufSize - 1 ] - 1 );
        }

        if ( ptrNext > ( std::uint8_t* )pxTail )
        {
            /* pxHead > pxTail. */ 
            topFreeSpace = ( std::size_t )( &pcBuf[ xBufSize - 1 ] - ptrNext ) + 1;

            bottomFreeSpace = ( std::size_t )( ( std::uint8_t* )pxTail - pcBuf );

            if( topFreeSpace >= sizeof( rbItem_t ) )
            {
                /* Fit header on top part. */
                pHeader = ptrNext; 

                topFreeSpace -= sizeof( rbItem_t );
            }
            else
            {
                /* Top free space not usable. */
                topFreeSpace = 0;
                
                if( bottomFreeSpace >= sizeof( rbItem_t ) )
                {
                    /* Fit header on bottom part. */
                    pHeader = pcBuf;

                    bottomFreeSpace -= sizeof( rbItem_t ); 
                }
            }
        }
        else
        {
            /* pxTail > pxHead. */
            topFreeSpace = ( std::size_t )( ( std::uint8_t* )pxTail - ptrNext );

            if( topFreeSpace >= ( sizeof( rbItem_t ) + xItemSize ) )
            {
                pHeader = ptrNext;
            }
        }

        /* Free space for new item check. */
        if( xItemSize > ( topFreeSpace + bottomFreeSpace ) )
        {
            pHeader = nullptr;
        }
    }

    return pHeader;
}

/**
 * @brief Inserts a new item in the ring buffer 
 *        at a specific location.
 * 
 * @note Private method.
 * 
 * @param[in] pxHeader New item position in the ring buffer.
 * @param[in] pxItem Item to insert.
 * @param[in] xItemSize Size [byte] of the item to insert.
 * @param[out] True when item successfully insertion.
 *
 */

void ringbuf::pushItem( const void* pxHeader, 
                        const void* pxItem, 
                        const std::size_t xItemSize ) 
{
    rbItem_t xNewItem;
    std::size_t topPartSize = 0;
    std::size_t bottomPartSize = 0;

    /* Determine where new item shall be copied. */
    const std::uint8_t* pDataDst = ( const std::uint8_t* )pxHeader + sizeof( rbItem_t );

    /* Buffer roll-over check. */
    if( pDataDst > &pcBuf[ xBufSize - 1 ] )
    {
        pDataDst = pcBuf;
    }

    if ( pDataDst >= ( std::uint8_t* )pxTail )
    {
        /* Write on top part of the buffer. */
        topPartSize = ( std::size_t )( &pcBuf[ xBufSize - 1 ] - pDataDst ) + 1;

        /* Buffer roll-over check. */
        if( xItemSize > topPartSize )          
        {
            bottomPartSize = xItemSize - topPartSize;
        }
    }
    else
    {
        topPartSize = 0;
        bottomPartSize = xItemSize;
    }

    /* Copy first part of data. */
    std::memcpy( ( void* )pDataDst, pxItem, topPartSize );

    pDataDst += topPartSize;

    /* Buffer roll-over. */
    if( pDataDst == &pcBuf[ xBufSize ] )
    {
        pDataDst = pcBuf;
    }

    /* Copy second part of data. */
    std::memcpy( ( void* )pDataDst, ( std::uint8_t* )pxItem + topPartSize , bottomPartSize );

    /* Copy item header. */
    xNewItem.pxNext = ( rbItem_t* )pxHeader;
    xNewItem.pxPrev = pxHead;
    xNewItem.xItemSize = xItemSize;

    std::memcpy( ( void* )pxHeader, &xNewItem, sizeof( rbItem_t ) );

    /* Update Head*/
    pxHead->pxNext = ( rbItem_t* )pxHeader;
    pxHead = ( rbItem_t* )pxHeader;

    ++xTotItemCnt;
}

/*--------------------- Public methods ---------------------*/

/**
 * @brief Ring buffer constructor.
 *
 * @param[in] pcPool Pointer to the memery pool used by this ringBuf instance.
 * @param[in] ulPoolSize Size of the memory pool.
 *
 */

ringbuf::ringbuf( std::uint8_t* pcPool, 
                  const std::size_t xPoolSize ) : 
                  pcBuf( pcPool ), 
                  xBufSize( xPoolSize ) 
{ 
    /* Reset buffer. */
    std::memset( ( void* )pcBuf, 0, xBufSize );

    reset();
} 

/**
 * @brief Discards all data in the ring buffer.
 *
 */

void ringbuf::flush( void )
{
    reset();
}

 /**
 * @brief Inserts a new item in the ring buffer.
 *
 * @param[in] pxItem Pointer to the item to insert.
 * @param[in] xItemSize Size of the item to insert.
 * @param[out] True when item successfully insertion.
 *
 */

bool ringbuf::push( const void* pxItem, 
                    const std::size_t xItemSize ) 
{
    bool isItemPushed = false;
    std::uint8_t* pHeader = nullptr;

    if(    ( xItemSize > 0 ) \
        && ( xItemSize < ( xBufSize - sizeof( rbItem_t ) ) )    )

    {        
        pHeader = getNextPtr( xItemSize );

        /* Remove old items if space is not enough. */
        while( pHeader == nullptr )
        {
            deleteTail();

            pHeader = getNextPtr( xItemSize );
        }

        pushItem( pHeader, pxItem,xItemSize );

        isItemPushed = true;
    }

    return isItemPushed;
}


/**
 * @brief Checks whether the ring buffer is empty.
 *
 * @param[out] True when empty.
 *
 */

bool ringbuf::isEmpty( void ) 
{
    return ( xTotItemCnt == 0 );
}

/**
 * @brief Deletes the head (most recent item) of the ring buffer.
 *
 * @param[out] True when the head is successfully deleted.
 *
 */

bool ringbuf::deleteHead( void ) 
{
    bool isHeadDeleted = false;

    if( xTotItemCnt > 0 )
    {
        pxHead = pxHead->pxPrev;
        pxHead->pxNext = pxHead;

        if( --xTotItemCnt == 0 )
        {
            reset();
        }

        isHeadDeleted = true;
    }

    return isHeadDeleted;
}

/**
 * @brief Deletes the tail (oldest item) of the ring buffer.
 *
 * @param[out] True when the tail is successfully deleted.
 *
 */

bool ringbuf::deleteTail( void )
{
    bool isTailDeleted = false;

    if( xTotItemCnt > 0 )
    {
        pxTail = pxTail->pxNext;
        pxTail->pxPrev = pxTail;

        if( --xTotItemCnt == 0 )
        {
            reset();
        }
        
        isTailDeleted = true;
    }

    return isTailDeleted;
}

/**
 * @brief Copies data of the specified item into the destination buffer.
 *
 * @param[in] pxItem Pointer to the item to retrieve.
 * @param[in] pcDstBuf Destination buffer where data is copied.
 * @param[out] True when data successfully copied.
 *
 */

bool ringbuf::getData( const rbItem_t* pxItem, 
                       std::uint8_t* pcDstBuf )
{
    bool isDataCopied = false;

    std::size_t size = 0;
    std::size_t rolloversize = 0;

    if( ( pxItem != nullptr) && ( pxItem->xItemSize > 0 ) )
    { 
        const std::uint8_t* pItemData = ( const std::uint8_t* )pxItem + sizeof( rbItem_t );

        /* Buffer roll-over. */
        if( pItemData > &pcBuf[ xBufSize - 1 ] )
        {
            pItemData = pcBuf;
            size = pxItem->xItemSize;
        }

        if( ( pItemData + pxItem->xItemSize  ) <= &pcBuf[ xBufSize - 1 ]  )
        {
            size = pxItem->xItemSize;
        }
        else
        {
            size = ( std::size_t )( &pcBuf[ xBufSize - 1 ] - pItemData ) + 1;

            rolloversize = pxItem->xItemSize - size;
        }

        memcpy( pcDstBuf, pItemData, size );
        memcpy( (pcDstBuf + size), pcBuf, rolloversize );

        isDataCopied = true;
    }

    return isDataCopied;
}

/**
 * @brief Gets the size of data in the head of the ring buffer.
 *
 * @param[out] Head size.
 *
 */

const std::size_t ringbuf::getHeadSize( void ) 
{
    return pxHead->xItemSize;
}

/**
 * @brief Gets the size of data in the tail of the ring buffer.
 *
 * @param[out] Tail data size.
 *
 */

const std::size_t ringbuf::getTailSize( void ) 
{
    return pxTail->xItemSize;
}

/**
 * @brief Gets the number of items present in the ring buffer.
 *
 * @param[out] Items count.
 *
 */

const std::size_t ringbuf::getItemsCnt( void ) 
{
    return xTotItemCnt;
}

/**
 * @brief Returns a pointer to the head of the ring buffer.
 *
 * @param[out] pointer to head
 *
 */

const rbItem_t* ringbuf::getHead( void ) 
{
    return pxHead;
}

/**
 * @brief Returns a pointer to the tail of the ring buffer.
 *
 * @param[out] pointer to head
 *
 */

const rbItem_t* ringbuf::getTail( void ) 
{
    return pxTail;
}

/*---------------------------------------------------------------------------*/
