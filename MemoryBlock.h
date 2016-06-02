#pragma once
#include <stdlib.h>
#include <string.h>
typedef unsigned short USHORT;
struct MemoryBlock
{
	//链表前后指针
	MemoryBlock*prev;
	MemoryBlock* next;
	//分配MemoryBlock总大小。
	int mSize;
	//未分配块个数
	int free;
	//首个未分配块索引
	int first;
	//Padding
	char mPad[2];
	MemoryBlock(USHORT=1, USHORT = 0);
	static void* operator new(size_t, USHORT, USHORT);
	static void operator delete(void*);
};

