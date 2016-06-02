#include "MemoryPool.h"
#include "MemoryBlock.h"
#include <string.h>
#include <assert.h>

MemoryPool* MemoryPool::memoryPoolInstance = nullptr;
USHORT MemoryPool::poolMapIndex = 0;

MemoryPool::MemoryPool(int initBlockSize /* = 1024 */, int growBlockSize /* = 256 */)
	:initBlockCount(initBlockSize),
	growBlockcount(growBlockSize),
	firstIndex(0),
	lastIndex(0),
	mpPtr(nullptr) {
	memset(memoryHashMap, 0, sizeof(memoryHashMap));
}

int MemoryPool::Hash(int value)
{
	assert(value > 0);
	int hashCode = poolMapIndex;
	if (hashCode > 0 && (value == poolMap[hashCode - 1]))
	{
		return hashCode - 1;
	}
	poolMap[poolMapIndex++] = value;
	return hashCode;
}

int MemoryPool::AlignBytes(int by)
{
	assert(by > 0);
	if (by <= 4)
	{
		return 4;
	}
	int alignedBytes = (by + POOLALIGNMENT - 1)&(~(POOLALIGNMENT - 1));
	return alignedBytes;
}

int MemoryPool::GetUnitSize(int index)
{
	int unitSize = poolMap[index];
	assert(unitSize != 0 && "unitSize can not be zero");
	return unitSize;
}

MemoryPool* MemoryPool::GetInstance()
{
	if (memoryPoolInstance == nullptr)
	{
		memoryPoolInstance = new MemoryPool();
	}
	return memoryPoolInstance;
}

void* MemoryPool::Alloc(int bytes)
{
	int alignedByte = AlignBytes(bytes);
	MemoryBlock* block = nullptr;
	MemoryBlock* blockHead = nullptr;
	int hashCode = Hash(alignedByte);
	
	block = memoryHashMap[hashCode];
	if (block == nullptr)
	{
		assert(memoryHashMap[hashCode] == nullptr);
		memoryHashMap[hashCode] = new (initBlockCount, alignedByte)MemoryBlock(initBlockCount, alignedByte);
		block = memoryHashMap[hashCode];
		firstIndex = hashCode;
		if (hashCode >= firstIndex)
		{
			lastIndex = hashCode;
		}
		return block->mPad + 2;
	}
	blockHead = block;
	while(block && !block->free)
	{
		/*if (block->next == nullptr)
		{
			blockHead = block;
		}*/
		block = block->next;
	}

	if (block)
	{
		char* ptr = block->mPad + (block->first*(alignedByte+2));
		block->first = *((USHORT*)ptr);
		--block->free;
		return (ptr);
	}
	else
	{
		if (growBlockcount == 0)
		{
			return nullptr;
		}
		block = new(growBlockcount, alignedByte)MemoryBlock(growBlockcount, alignedByte);
		if (block == nullptr)
		{
			return nullptr;
		}
		block->next = blockHead;
		blockHead->prev = block;
		memoryHashMap[hashCode] = block;

		return block->mPad + 2;
	}
}
void MemoryPool::FreeAlloc(void* p)
{
	unsigned long* pBlock = (unsigned long*)p;
	int unitSize = 0;
	int indexer = 0;
	MemoryBlock* tmp = nullptr;
	for (int i = 0; i < 16;i++)
	{
		tmp = memoryHashMap[i];
		if (memoryHashMap[i] != nullptr)
		{
			while (tmp != nullptr)
			{
				if ((unsigned long*)tmp < (unsigned long*)pBlock &&
					(unsigned long*)pBlock < (unsigned long*)((char*)tmp + tmp->mSize))
				{
					unitSize = GetUnitSize(i);
					indexer = i;
					break;
				}
				tmp = tmp->next;
			}
			if (indexer != 0)
			{
				break;
			}
		}
	}
	if (tmp == nullptr)
	{
		return;
	}

	*(USHORT*)p = tmp->first;
	tmp->first = ((char*)pBlock - (char*)tmp->mPad) / (unitSize+2);
	++tmp->free;
	if ((unitSize+2)* tmp->free == tmp->mSize)
	{
		//delete
		if (tmp->prev == nullptr && tmp->next != nullptr)
		{
			memoryHashMap[indexer] = tmp->next;
			tmp->next = nullptr;
			tmp->prev == nullptr;
		}
		else if (tmp->prev != nullptr && tmp->next != nullptr)
		{
			tmp->prev->next = tmp->next;
			tmp->next->prev = tmp->prev;
			tmp->next = nullptr;
			tmp->prev = nullptr;
		}
		else if (tmp->next == nullptr && tmp->prev != nullptr)
		{
			tmp->prev->next = tmp->next;
			tmp->next = nullptr;
			tmp->prev = nullptr;
		}
		delete tmp;
		memoryHashMap[indexer] = nullptr;
	}
	else
	{
		//do nothing
	}

}
MemoryPool::~MemoryPool()
{
	for (auto it : memoryHashMap)
	{
		if (it != nullptr)
		{
			delete it;
		}
	}
}