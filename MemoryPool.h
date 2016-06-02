#pragma once
#include <stdlib.h>
#include <map>

typedef unsigned short USHORT;
struct MemoryBlock;


//全局内存池为单例类。使用的时候只需要一个实例即可，并不需要多个实例。
class MemoryPool
{
private:
	static USHORT poolMapIndex;
	//分配不同内存块时其对应的映射表
	std::map<int, int>poolMap;
	//内存池对齐大小。
	const int POOLALIGNMENT = 8;
	//初始化内存块
	int initBlockCount;
	//内存块不足增长的块数。
	int growBlockcount;
	//首子内存池下标
	unsigned firstIndex;
	//末内存池下标。
	unsigned lastIndex;
	//最多16中不同内存块大小，也就是说子内存池最多有16个。
	MemoryBlock* memoryHashMap[16];
	MemoryBlock** mpPtr;
	//计算不同内存块对应的hashCode
	int Hash(int);
	//对齐字节
	int AlignBytes(int);
	//返回分配块大小。
	int GetUnitSize(int);
protected:
	static MemoryPool* memoryPoolInstance;
	MemoryPool(int initBlockSize = 1024, int growBlockSize = 256);
public:
	//分配内存
	void* Alloc(int);
	//释放内存。
	void FreeAlloc(void*);
	//返回全局内存池实例
	static MemoryPool* GetInstance();
	~MemoryPool();
};
