#ifndef __MEMPOOL_H__
#define __MEMPOOL_H__

class CMemPool
{
private:
	//The purpose of the structure`s definition is that we can operate linkedlist conveniently
	struct _Unit                     //The type of the node of linkedlist.
	{
		struct _Unit *pPrev, *pNext;
	};

	void* m_pMemBlock;                //The address of memory pool.

	//Manage all unit with two linkedlist.
	struct _Unit*    m_pAllocatedMemBlock; //Head pointer to Allocated linkedlist.
	struct _Unit*    m_pFreeMemBlock;      //Head pointer to Free linkedlist.

	unsigned long    m_ulUnitSize; //Memory unit size. There are much unit in memory pool.
	unsigned long    m_ulBlockSize;//Memory pool size. Memory pool is make of memory unit.

	unsigned long    m_alloced;
public:
	CMemPool(unsigned long lUnitNum = 50, unsigned long lUnitSize = 1024);
	~CMemPool();

	void* Alloc(unsigned long ulSize, bool bUseMemPool = true); //Allocate memory unit
	void Free(void* p);                                   //Free memory unit
	unsigned long size();
};

#endif //__MEMPOOL_H__