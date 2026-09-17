#include "ring_buffer.hpp"
#include "CppUTest/TestHarness.h"

TEST_GROUP(RingBufferInt)
{
};

TEST(RingBufferInt, PushPop)
{
    ring_buffer<int, 2> rb;
    rb.push(1);
    rb.push(2);

    LONGS_EQUAL(1, rb.pop());
	LONGS_EQUAL(2, rb.pop());
}

TEST(RingBufferInt, GetCount)
{
	ring_buffer<int, 20> rb;

	for(int i = 0; i < 50; i++)
	{
		rb.push(i);
	}

	UNSIGNED_LONGS_EQUAL(20, rb.get_count());

	for(int i = 0; i < 10; i++)
	{
		rb.pop();
	}

	UNSIGNED_LONGS_EQUAL(10, rb.get_count());
}

TEST(RingBufferInt, OverwritesTheOldestValueWhenFull)
{
	ring_buffer<int, 3> rb;

	rb.push(1);
	rb.push(2);
	rb.push(3);
	rb.push(4);	// overwrites the 1

	LONGS_EQUAL(2, rb.pop());
	LONGS_EQUAL(3, rb.pop());
	LONGS_EQUAL(4, rb.pop());
	CHECK_TRUE(rb.is_empty());
}

TEST(RingBufferInt, PopOnAnEmptyBufferReturnsADefaultValue)
{
	ring_buffer<int, 4> rb;

	CHECK_TRUE(rb.is_empty());
	LONGS_EQUAL(0, rb.pop());
	UNSIGNED_LONGS_EQUAL(0, rb.get_count());
}