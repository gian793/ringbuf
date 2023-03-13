#include "CppUTest/TestHarness.h"

#include <iostream>
#include <cstring>

extern "C"
{
	/*
	 * Add your c-only include files here
	 */
}

#include "ringbuf.hpp"
		
TEST_GROUP( ringbuf )
{
    void setup()
    {	
		//MemoryLeakWarningPlugin::saveAndDisableNewDeleteOverloads();
    }

    void teardown()
    {
		//MemoryLeakWarningPlugin::restoreNewDeleteOverloads();
    }
};



TEST( ringbuf, declaration)
{
	/*
	* TEST data. 
	*
	*/
	
	constexpr uint32_t mem_pool_size = 128U;
	
	uint8_t memPool[ mem_pool_size ]; 

	ringbuf testBuf( memPool, mem_pool_size );

	
	
	/*
	* TEST sequence. 
	*
	*/
	
	
	/* Buffer is empty. */
	CHECK_TRUE( testBuf.isEmpty() );
	
	/* Tail cannot be deleted. */
	CHECK_FALSE( testBuf.deleteTail() );
	
	/* No items present. */
	CHECK_EQUAL( 0, testBuf.getItemsCnt() );

	/* Head size is zero. */
	CHECK_EQUAL( 0, testBuf.getHeadSize() );
	
	/* Tail size is zero. */
	CHECK_EQUAL( 0, testBuf.getTailSize() );
	

}

	


TEST( ringbuf, push_1_item)
{
	/*
	* TEST data. 
	*
	*/
	
	constexpr uint32_t mem_pool_size = 128U;
	
	uint8_t memPool[ mem_pool_size ]; 
	
	ringbuf testBuf( memPool, mem_pool_size );
	uint8_t  testItem1[ ] = { 1, 2, 3, 4, 5};
	
	
	
	/*
	* TEST sequence. 
	*
	*/
	
	
	
	/* Push item1. */
	CHECK( testBuf.push( testItem1, sizeof( testItem1 ) ) );

	/* Buffer is not empty now. */
	CHECK_FALSE( testBuf.isEmpty() );
	
	/* CHeck head size. */
	CHECK_EQUAL( sizeof( testItem1 ), testBuf.getHeadSize() );
	
	/* Tail size == head size. */
	CHECK_EQUAL( testBuf.getTailSize(), testBuf.getHeadSize() );
	
	/* Check number of items. */
	CHECK_EQUAL( 1, testBuf.getItemsCnt() );
	
	
	
	/* Remove item1 with delete tail. */
	CHECK_TRUE( testBuf.deleteTail() );
	
	/* No items present. */
	CHECK_EQUAL( 0, testBuf.getItemsCnt() );
	
	/* Head size is zero. */
	CHECK_EQUAL( 0, testBuf.getHeadSize() );
	
	/* Tail size is zero. */
	CHECK_EQUAL( 0, testBuf.getTailSize() );
	
	
	
	/* Push item1 in again. */
	CHECK( testBuf.push( testItem1, sizeof( testItem1 ) ) );
	
	/* Buffer is not empty now. */
	CHECK_FALSE( testBuf.isEmpty() );
	
	/* CHeck head size. */
	CHECK_EQUAL( sizeof( testItem1 ), testBuf.getHeadSize() );
	
	/* Tail size == head size. */
	CHECK_EQUAL( testBuf.getTailSize(), testBuf.getHeadSize() );
	
	/* Check number of items. */
	CHECK_EQUAL( 1, testBuf.getItemsCnt() );
	
	
	
	/* Remove item1 with delete head. */
	CHECK_TRUE( testBuf.deleteHead() );
	
	/* No items present. */
	CHECK_EQUAL( 0, testBuf.getItemsCnt() );
	
	/* Head size is zero. */
	CHECK_EQUAL( 0, testBuf.getHeadSize() );
	
	/* Tail size is zero. */
	CHECK_EQUAL( 0, testBuf.getTailSize() );
	
	
	
	/* Push item1 in again. */
	CHECK( testBuf.push( testItem1, sizeof( testItem1 ) ) );
	
	/* Buffer is not empty now. */
	CHECK_FALSE( testBuf.isEmpty() );
	
}
	

TEST( ringbuf, push_2_items)
{
	/*
	* TEST data. 
	*
	*/

	constexpr uint32_t mem_pool_size = 70000U;
	
	uint8_t memPool[ mem_pool_size ]; 
	
	ringbuf testBuf( memPool, mem_pool_size );
	
	uint8_t  testItem1[ ] = { 1, 2, 3, 4, 5};
	uint16_t testItem2[ 33000 ] = { 0 };
	
	
	/*
	* TEST sequence. 
	*
	*/
	
	
	/* Push items. */
	testBuf.push( testItem1, sizeof( testItem1 ) );
	testBuf.push( testItem2, sizeof( testItem2 ) );
	
	/* Check number of items. */
	CHECK_EQUAL( 2, testBuf.getItemsCnt() );
	
	/* Check head size. */
	CHECK_EQUAL( sizeof( testItem2 ), testBuf.getHeadSize() );
	
	/* Check tail size. */
	CHECK_EQUAL( sizeof( testItem1 ), testBuf.getTailSize() );
	
}


TEST( ringbuf, push_2_items_rollover)
{
	/*
	* TEST data. 
	*
	*/

	constexpr uint32_t mem_pool_size = 54U;
	
	uint8_t memPool[ mem_pool_size ]; 
	
	ringbuf testBuf( memPool, mem_pool_size );
	
	uint8_t testItem1[ 6 ];
	uint8_t testItem2[ ] = { 1, 2, 3, 4, 5, 6 };
	
	
	/*
	* TEST sequence. 
	*
	*/
	
	
	/* Push items. */
	testBuf.push( testItem1, sizeof( testItem1 ) );
	testBuf.push( testItem2, sizeof( testItem2 ) );
	
	/* Check number of items. */
	CHECK_EQUAL( 1, testBuf.getItemsCnt() );
	
	/* Check head size. */
	CHECK_EQUAL( sizeof( testItem2 ), testBuf.getHeadSize() );
	
	/* Check tail size. */
	CHECK_EQUAL( sizeof( testItem2 ), testBuf.getTailSize() );
	
}



TEST( ringbuf, push_3_items)
{
	/*
	* TEST data. 
	*
	*/

	constexpr uint32_t mem_pool_size = 94;

	uint8_t memPool[ mem_pool_size ]; 
	
	ringbuf testBuf( memPool, mem_pool_size );
	
	uint8_t  testItem1[ ] = { 1, 2, 3, 4, 5};
	uint16_t testItem2[ ] = { 101, 102, 103, 104, 105, 106, 107};
	uint8_t testItem3[ ] = { 1, 2, 3};
	
	
	/*
	* TEST sequence. 
	*
	*/
	
	
	/* Push items. */
	testBuf.push( testItem1, sizeof( testItem1 ) );
	testBuf.push( testItem2, sizeof( testItem2 ) );
	testBuf.push( testItem3, sizeof( testItem3 ) );
	
	/* Check number of items. */
	CHECK_EQUAL( 3, testBuf.getItemsCnt() );
	
	/* Check head size. */
	CHECK_EQUAL( sizeof( testItem3 ), testBuf.getHeadSize() );
	
	/* Check tail size. */
	CHECK_EQUAL( sizeof( testItem1 ), testBuf.getTailSize() );
}


TEST( ringbuf, push_3_items_rollover)
{
	/*
	* TEST data. 
	*
	*/

	constexpr uint32_t mem_pool_size = 86U;

	uint8_t memPool[ mem_pool_size ]; 
	
	ringbuf testBuf( memPool, mem_pool_size );
	
	uint8_t testItem1[ 8 ];
	uint8_t testItem2[ 6 ];
	uint8_t testItem3[ 10 ];
	
	
	/*
	* TEST sequence. 
	*
	*/
	
	
	/* Push items. */
	testBuf.push( testItem1, sizeof( testItem1 ) );
	testBuf.push( testItem2, sizeof( testItem2 ) );
	testBuf.push( testItem3, sizeof( testItem3 ) );
	
	/* Check number of items. */
	CHECK_EQUAL( 2, testBuf.getItemsCnt() );
	
	/* Check head size. */
	CHECK_EQUAL( sizeof( testItem3 ), testBuf.getHeadSize() );
	
	/* Check tail size. */
	CHECK_EQUAL( sizeof( testItem2 ), testBuf.getTailSize() );
}


TEST( ringbuf, push_4_items)
{
	/*
	* TEST data. 
	*
	*/
	
	constexpr uint32_t mem_pool_size = 100;
	
	uint8_t memPool[ mem_pool_size ]; 
	
	ringbuf testBuf( memPool, mem_pool_size );

	uint8_t testItem1[ ] = { 1, 2, 3, 4, 5, 6, 7, 8};
	uint8_t testItem2[ ] = { 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
	uint8_t testItem3[ ] = { 21, 22, 23, 24, 25, 26, 27, 28, 29, 30};
	uint8_t testItem4[ ] = { 1, 2, 3};
	
	
	/*
	* TEST sequence. 
	*
	*/
	
	
	/* Push items. */
	testBuf.push( testItem1, sizeof( testItem1 ) );
	testBuf.push( testItem2, sizeof( testItem2 ) );
	testBuf.push( testItem3, sizeof( testItem3 ) );

	/* Check number of items. */
	CHECK_EQUAL( 3, testBuf.getItemsCnt() );
	
	/* Check head size. */
	CHECK_EQUAL( sizeof( testItem3 ), testBuf.getHeadSize() );
	
	/* Check tail size. */
	CHECK_EQUAL( sizeof( testItem1 ), testBuf.getTailSize() );
	
	/* Push item4. */
	CHECK( testBuf.push( testItem4, sizeof( testItem4 ) ) );
	
	/* Check number of items, old items all removed because lack of space. */
	CHECK_EQUAL( 3, testBuf.getItemsCnt() );

	/* Check head size. */
	CHECK_EQUAL( sizeof( testItem4 ), testBuf.getHeadSize() );

	/* Check tail size. */
	CHECK_EQUAL( sizeof( testItem2 ), testBuf.getTailSize() );
}


TEST( ringbuf, push_5_items)
{
	/*
	* TEST data. 
	*
	*/
	
	constexpr uint32_t mem_pool_size = 100;
	
	uint8_t memPool[ mem_pool_size ]; 
	
	ringbuf testBuf( memPool, mem_pool_size );
	
	constexpr uint32_t buf5_size = 70;

	uint8_t testItem1[ ] = { 1, 2, 3, 4, 5, 6, 7, 8};
	uint8_t testItem2[ ] = { 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
	uint8_t testItem3[ ] = { 21, 22, 23, 24, 25, 26, 27, 28, 29, 30};
	uint8_t testItem4[ ] = { 1, 2, 3};
	uint8_t testItem5[ buf5_size ];
	
	
	/*
	* TEST sequence. 
	*
	*/
	
	/* Push items. */
	testBuf.push( testItem1, sizeof( testItem1 ) );
	testBuf.push( testItem2, sizeof( testItem2 ) );
	testBuf.push( testItem3, sizeof( testItem3 ) );
	testBuf.push( testItem4, sizeof( testItem4 ) );
	testBuf.push( testItem5, sizeof( testItem5 ) );
	
	/* Check number of items, old items all removed because lack of space. */
	CHECK_EQUAL( 1, testBuf.getItemsCnt() );

	/* Check head size. */
	CHECK_EQUAL( sizeof( testItem5 ), testBuf.getHeadSize() );

	/* Check tail size. */
	CHECK_EQUAL( sizeof( testItem5 ), testBuf.getTailSize() );
}

TEST( ringbuf, roll_over_items)
{
	/*
	* TEST data. 
	*
	*/
	
	constexpr uint32_t mem_pool_size = 100;
	
	uint8_t memPool[ mem_pool_size ]; 
	
	ringbuf testBuf( memPool, mem_pool_size );
	
	constexpr uint32_t buf_size = 51;

	uint8_t testItem1[ buf_size ];
	uint8_t testItem2[ ] = { 1 };
	uint8_t testItem3[ buf_size + 1 ];
	
	
	/*
	* TEST sequence. 
	*
	*/
	
	/* Push items. */
	testBuf.push( testItem1, sizeof( testItem1 ) );
	testBuf.push( testItem2, sizeof( testItem2 ) );
	
	/* Check number of items. */
	CHECK_EQUAL( 2, testBuf.getItemsCnt() );

	/* Check head size. */
	CHECK_EQUAL( sizeof( testItem2 ), testBuf.getHeadSize() );

	/* Check tail size. */
	CHECK_EQUAL( sizeof( testItem1 ), testBuf.getTailSize() );

	/* Push item3. */
	CHECK( testBuf.push( testItem3, sizeof( testItem3 ) ) );

	/* Check number of items, tail removed because of lack of space. */
	CHECK_EQUAL( 1, testBuf.getItemsCnt() );
}


TEST( ringbuf, flush )
{
	/*
	* TEST data. 
	*
	*/

	constexpr size_t  mem_pool_size = 256U;
	uint8_t memPool[ mem_pool_size  ];

	ringbuf  testRBuf( memPool, mem_pool_size );
	
	uint16_t item1[ ] = { 1, 2, 3, 4, 5 };
	uint8_t  item2[ ] = { 6, 7, 8 };
	uint32_t item3[ ] = { 21, 22, 23, 24 };
	
	testRBuf.push( item1, sizeof( item1 ) );
	testRBuf.push( item2, sizeof( item2 ) );
	testRBuf.push( item3, sizeof( item3 ) );


	/*
	* TEST sequence. 
	*
	*/
	


	CHECK_EQUAL( 3, testRBuf.getItemsCnt() );

	testRBuf.flush();

	CHECK_EQUAL( 0, testRBuf.getItemsCnt() );

	CHECK_EQUAL( 0, testRBuf.getTailSize() );

	CHECK_EQUAL( 0, testRBuf.getHeadSize() );

	CHECK_TRUE( testRBuf.getTail() == testRBuf.getHead() );

	CHECK_TRUE( testRBuf.getTail()->pxNext == testRBuf.getHead()->pxPrev );

	CHECK_TRUE( testRBuf.getTail()->pxPrev == testRBuf.getHead()->pxNext );
}


TEST( ringbuf, get_head_tail )
{
	/*
	* TEST data. 
	*
	*/
	
	constexpr uint32_t mem_pool_size = 82;
	
	uint8_t memPool[ mem_pool_size ]; 
	
	ringbuf testBuf( memPool, mem_pool_size );

	uint8_t testItem1[  ] = {1, 2, 3, 4, 5, 6};
	uint8_t testItem2[  ] = { 7 };
	uint8_t testItem3[  ] = { 8, 9, 10 };
	
	
	/*
	* TEST sequence. 
	*
	*/
	
	/* Push items. */
	testBuf.push( testItem1, sizeof( testItem1 ) );
	testBuf.push( testItem2, sizeof( testItem2 ) );
	testBuf.push( testItem3, sizeof( testItem3 ) );

	/* Check number of items. */
	CHECK_EQUAL( 3, testBuf.getItemsCnt() );

	/* Check head. */
	const rbItem_t* pHead = testBuf.getHead();
	CHECK_EQUAL( sizeof( testItem3 ), pHead->xItemSize );
	CHECK_EQUAL( sizeof( testItem2 ), pHead->pxPrev->xItemSize );

	/* Check tail. */
	const rbItem_t* pTail = testBuf.getTail();
	CHECK_EQUAL( sizeof( testItem1 ), pTail->xItemSize);
	CHECK_EQUAL( sizeof( testItem2 ), pTail->pxNext->xItemSize );

	/* Prev pointer checks. */
	CHECK_TRUE( ( pHead->pxPrev == pTail->pxNext ) );
	CHECK_TRUE( ( pHead->pxPrev->pxPrev == pTail ) );
	CHECK_TRUE( ( pHead->pxPrev->pxPrev->pxPrev == pTail ) );
	CHECK_TRUE( ( pHead->pxPrev->pxPrev->pxPrev == pTail->pxPrev ) );

	/* Next pointer checks. */
	CHECK_TRUE( ( pHead->pxPrev == pTail->pxNext ) );
	CHECK_TRUE( ( pTail->pxNext->pxNext == pHead ) );
	CHECK_TRUE( ( pTail->pxNext->pxNext->pxNext == pHead ) );
	CHECK_TRUE( ( pTail->pxNext->pxNext->pxNext == pHead->pxNext ) );
}


TEST( ringbuf, getDataEmpty )
{
	/*
	* TEST data. 
	*
	*/
	
	constexpr uint32_t mem_pool_size = 100;
	
	uint8_t memPool[ mem_pool_size ]; 

	uint8_t dataBuf1[ 10 ] = {0};

	
	ringbuf testBuf( memPool, mem_pool_size );
	
	
	/*
	* TEST sequence. 
	*
	*/

	const rbItem_t* pHead = testBuf.getHead();

	/* Empty ring buf. */
	CHECK_FALSE( testBuf.getData( pHead, dataBuf1 ));
}


TEST( ringbuf, getData_1 )
{
	/*
	* TEST data. 
	*
	*/
	
	constexpr uint32_t mem_pool_size = 100;
	constexpr uint32_t databuf1_size = 10;
	//constexpr uint32_t databuf2_size = 5;
	
	uint8_t memPool[ mem_pool_size ]; 


	uint8_t dataBuf1[ databuf1_size ] = {0};
	// uint8_t dataBuf2[ databuf2_size ];
	
	ringbuf testBuf( memPool, mem_pool_size );

	uint8_t testItem1[  ] = {1, 2, 3, 4, 5, 6};
	
	
	/*
	* TEST sequence. 
	*
	*/


	testBuf.push( testItem1, sizeof( testItem1 ) );

	const rbItem_t* pHead = testBuf.getHead();
	testBuf.getData( pHead, dataBuf1 );

	/* Get data. */
	CHECK_EQUAL( 0, memcmp( testItem1, dataBuf1, sizeof( testItem1 ) ) );
}


TEST( ringbuf, getData_2 )
{
	/*
	* TEST data. 
	*
	*/
	

	constexpr uint32_t mem_pool_size = 128;
	
	uint8_t memPool[ mem_pool_size ]; 
	
	ringbuf testBuf( memPool, mem_pool_size );

	uint8_t testItem1[ ] = { 1, 2, 3, 4, 5, 6, 7, 8};
	uint8_t testItem2[ ] = { 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
	uint8_t testItem3[ ] = { 21, 22, 23, 24, 25};
	uint8_t testItem4[ ] = { 1, 2, 3};

	uint8_t dataBuf1[ 10 ] = {0};
	uint8_t dataBuf2[ 10 ] = {0};
	uint8_t dataBuf3[ 10 ] = {0};
	uint8_t dataBuf4[ 10 ] = {0};
	


	/*
	* TEST sequence. 
	*
	*/

	testBuf.push( testItem1, sizeof( testItem1 ) );
	testBuf.push( testItem2, sizeof( testItem2 ) );
	testBuf.push( testItem3, sizeof( testItem3 ) );
	testBuf.push( testItem4, sizeof( testItem4 ) );

	/* Get Head data. */
	const rbItem_t* pHead = testBuf.getHead();
	testBuf.getData( pHead, dataBuf4 );

	CHECK_EQUAL( 0, memcmp( testItem4, dataBuf4, sizeof( testItem4 ) ) );

	/* Get Head->pxPrev data. */
	testBuf.getData( pHead->pxPrev, dataBuf3 );

	CHECK_EQUAL( 0, memcmp( testItem3, dataBuf3, sizeof( testItem3 ) ) );

	/* Get Tail data. */
	const rbItem_t* pTail = testBuf.getTail();
	testBuf.getData( pTail, dataBuf1 );

	CHECK_EQUAL( 0, memcmp( testItem1, dataBuf1, sizeof( testItem1 ) ) );

	/* Get Tail->pxNext data. */
	testBuf.getData( pTail->pxNext, dataBuf2 );

	CHECK_EQUAL( 0, memcmp( testItem2, dataBuf2, sizeof( testItem2 ) ) );
}


TEST( ringbuf, getData_rollover1 )
{
	/*
	* TEST data. 
	*
	*/
	
	/* All data of 3rd item rolls-over. */
	constexpr uint32_t mem_pool_size = 84U;
	
	uint8_t memPool[ mem_pool_size ]; 
	
	ringbuf testBuf( memPool, mem_pool_size );

	uint8_t testItem1[ ] = { 1, 2, 3, 4, 5, 6};
	uint8_t testItem2[ ] = { 11, 12, 13, 14, 15, 16};
	uint8_t testItem3[ ] = { 21, 22, 23, 24, 25};

	uint8_t dataHead[ 10 ] = {0};
	uint8_t dataTail[ 10 ] = {0};
	


	/*
	* TEST sequence. 
	*
	*/

	testBuf.push( testItem1, sizeof( testItem1 ) );
	testBuf.push( testItem2, sizeof( testItem2 ) );
	testBuf.push( testItem3, sizeof( testItem3 ) );

		
	/* Check number of items, first element removed. */
	CHECK_EQUAL( 2, testBuf.getItemsCnt() );

	/* Get Head & Tail data. */
	const rbItem_t* pHead = testBuf.getHead();
	const rbItem_t* pTail = testBuf.getTail();

	testBuf.getData( pHead, dataHead );
	testBuf.getData( pTail, dataTail );

	CHECK_EQUAL( 0, memcmp( testItem3, dataHead, sizeof( testItem3 ) ) );
	CHECK_EQUAL( 0, memcmp( testItem2, dataTail, sizeof( testItem2 ) ) );
}

TEST( ringbuf, getData_rollover2 )
{
	/*
	* TEST data. 
	*
	*/
	
	/* Data of 3rd item partially rolls-over. */
	constexpr uint32_t mem_pool_size = 86U;
	
	uint8_t memPool[ mem_pool_size ]; 
	
	ringbuf testBuf( memPool, mem_pool_size );

	uint8_t testItem1[ ] = { 1, 2, 3, 4, 5, 6};
	uint8_t testItem2[ ] = { 11, 12, 13, 14, 15, 16};
	uint8_t testItem3[ ] = { 21, 22, 23, 24, 25};

	uint8_t dataHead[ 10 ] = {0};
	uint8_t dataTail[ 10 ] = {0};
	

	/*
	* TEST sequence. 
	*
	*/

	testBuf.push( testItem1, sizeof( testItem1 ) );
	testBuf.push( testItem2, sizeof( testItem2 ) );
	testBuf.push( testItem3, sizeof( testItem3 ) );

		
	/* Check number of items, first element removed. */
	CHECK_EQUAL( 2, testBuf.getItemsCnt() );

	/* Get Head & Tail data. */
	const rbItem_t* pHead = testBuf.getHead();
	const rbItem_t* pTail = testBuf.getTail();

	testBuf.getData( pHead, dataHead );
	testBuf.getData( pTail, dataTail );

	CHECK_EQUAL( 0, memcmp( testItem3, dataHead, sizeof( testItem3 ) ) );
	CHECK_EQUAL( 0, memcmp( testItem2, dataTail, sizeof( testItem2 ) ) );
}


TEST( ringbuf, getData_rollover3 )
{
	/*
	* TEST data. 
	*
	*/
	
	/* All buffer filled and then 1 item added. */
	constexpr uint32_t mem_pool_size = 88U;
	
	uint8_t memPool[ mem_pool_size ]; 
	
	ringbuf testBuf( memPool, mem_pool_size );

	uint8_t testItem1[ ] = { 1, 2, 3, 4, 5, 6};
	uint8_t testItem2[ ] = { 11, 12, 13, 14, 15, 16};
	uint8_t testItem3[ ] = { 21, 22, 23, 24};
	uint8_t testItem4[ ] = { 31, 32, 33, 34, 35, 36, 37, 38};

	uint8_t dataHead[ 10 ] = {0};
	uint8_t dataTail[ 10 ] = {0};
	

	/*
	* TEST sequence. 
	*
	*/

	testBuf.push( testItem1, sizeof( testItem1 ) );
	testBuf.push( testItem2, sizeof( testItem2 ) );
	testBuf.push( testItem3, sizeof( testItem3 ) );
	testBuf.push( testItem4, sizeof( testItem4 ) );

		
	/* Check number of items, first element removed. */
	CHECK_EQUAL( 2, testBuf.getItemsCnt() );

	/* Get Head & Tail data. */
	const rbItem_t* pHead = testBuf.getHead();
	const rbItem_t* pTail = testBuf.getTail();

	testBuf.getData( pHead, dataHead );
	testBuf.getData( pTail, dataTail );

	CHECK_EQUAL( 0, memcmp( testItem4, dataHead, sizeof( testItem4 ) ) );
	CHECK_EQUAL( 0, memcmp( testItem3, dataTail, sizeof( testItem3 ) ) );
}


TEST(ringbuf, getData_different_type )
{
	/*
	* TEST data. 
	*
	*/

	constexpr size_t  mem_pool_size = 1024U;
	uint8_t memPool[ mem_pool_size  ];

	ringbuf  myFirstRingBuf( memPool, mem_pool_size );
	
	uint16_t item1[ ] = { 1, 2, 3, 4, 5 };
	uint8_t  item2[ ] = { 6, 7, 8 };
	uint32_t item3[ ] = { 21, 22, 23, 24 };
	
	myFirstRingBuf.push( item1, sizeof( item1 ) );
	myFirstRingBuf.push( item2, sizeof( item2 ) );
	myFirstRingBuf.push( item3, sizeof( item3 ) );

	/*
	* TEST sequence. 
	*
	*/

	/* Get second item pushed. (Tail->Next)*/
	const rbItem_t* pItem2 = myFirstRingBuf.getTail()->pxNext;

	uint8_t rxBuf1[ sizeof( item1 ) ] = {0};
	uint8_t rxBuf2[ sizeof( item2 ) ] = {0};
	uint8_t rxBuf3[ sizeof( item3 ) ] = {0};

	myFirstRingBuf.getData( pItem2->pxPrev, rxBuf1 );
	myFirstRingBuf.getData( pItem2, rxBuf2 );
	myFirstRingBuf.getData( pItem2->pxNext, rxBuf3 );

	CHECK_EQUAL( 7,  rxBuf2[1]  );

	CHECK_EQUAL( 0, memcmp( item1, rxBuf1, sizeof( item1 ) ) );
	CHECK_EQUAL( 0, memcmp( item2, rxBuf2, sizeof( item2 ) ) );
	CHECK_EQUAL( 0, memcmp( item3, rxBuf3, sizeof( item3 ) ) );


}




	
	///////////////////// DEBUG /////////////////////
	/*
	std::cout << "Head size: " << testBuf.getHeadSize() << std::endl << std::endl;
	
	std::cout << "Tail size: " << testBuf.getTailSize() << std::endl << std::endl;
	
	std::cout << "Tot size: " << testBuf.getHeadSize() + testBuf.getTailSize() + 2*sizeof(rbItem_t) << std::endl << std::endl;
	*/

	
	/////////////////////////////////////////////////
	

	
	/*-------------------*/
	
