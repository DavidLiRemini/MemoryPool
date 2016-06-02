#include <vld.h>
#include <vector>
#include "List_Node.h"
#include "MemoryPool.h"
#include <stdlib.h>
#include <stdio.h>


MemoryPool* mp = MemoryPool::GetInstance();
class TestMemoryPool
{
private:
	int value1 = 10;
	int value2 = 20;
	float val = 5.9f;
public:
	TestMemoryPool(){}
	static void* operator new(size_t)
	{
		return mp->Alloc(sizeof(TestMemoryPool));
	}
	static void operator delete(void* p) {
		mp->FreeAlloc(p);
	}
};

class BigClassTest
{
private:
	char bc[500];
	int v = 10;
	char* p = nullptr;
public:
	BigClassTest() {
		//memset(bc, 0, sizeof(bc));
	}
	static void* operator new(size_t)
	{
		return mp->Alloc(sizeof(BigClassTest));
	}
	static void operator delete(void* p) {
		mp->FreeAlloc(p);
	}
};
class ThirdMemoryTest
{
private:
	int v[100];
	std::string s;
public:
	ThirdMemoryTest()
	{
		//memset(v, 0, sizeof(v));
		s = "";
	}
	static void* operator new(size_t)
	{
		return mp->Alloc(sizeof(ThirdMemoryTest));
	}
	static void operator delete(void* p)
	{
		mp->FreeAlloc(p);
	}
};

void test()
{
	TestMemoryPool* v = nullptr;
	for (int i = 0; i < 2050; i++)
	{
		v = new TestMemoryPool();
	}
	printf("testing\n");
	
	BigClassTest* b = nullptr;
	for (int i = 0; i < 3000;i++)
	{
		b = new BigClassTest();
	}

	ThirdMemoryTest* t = nullptr;
	for (int j = 0; j < 1920; j++)
	{
		t = new ThirdMemoryTest();
	}
	
}

int main()
{
	test();
	int p = 12;
	int*q = &p;
	int c = (int*)0x12345678 - (int*)0x12345670;
	short v = p;
	ListPool<int>iList;
	iList.AddNode(10);
	for (int i = 98; i > 1; i /= 10)
	{
		iList.AddNode(i);
	}
	
	printf("List size %u\n", iList.ListSize());
}