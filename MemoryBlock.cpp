#include "MemoryBlock.h"

MemoryBlock::MemoryBlock(USHORT initblock, USHORT unitSize)
	:prev(nullptr),
	next(nullptr),
	mSize(initblock* (unitSize+2)),
	free(initblock - 1),
	first(1)
{
	memset(mPad, 0, mSize - 20);
	char* padData = mPad;
	for (int i = 1; i< initblock;i++)
	{
		*reinterpret_cast<USHORT*>(padData) = i;
		padData += (unitSize+2);
	}
}

void* MemoryBlock::operator new(size_t, USHORT initBlock, USHORT unitSize)
{
	return ::operator new(sizeof(MemoryBlock) + initBlock* (unitSize+2));
}
void MemoryBlock::operator delete(void* p)
{
	::operator delete(p);
}