#pragma once
#include "TFJM.h"

/*
typedef s32(*QSCOMPARE)(const void* pa, const void* pb, const u32 param); // return values: pa<pb return -1, pa>pb return 1, pa==pb return 0   
template<typename T>
void QSort(T* pstart, T* pend, const u32 param, const QSCOMPARE comp)
{
	T  temp;
	T* pleft	= pstart	- 1;
	T* pright	= pend		+ 1;

	// Rien A Faire Si la taille du buffer est nulle
	if (pstart >= pend)
		return;

	// Otherwise, parsing array one time from right to left, and another time from left to right
	// to look for some miss placed element which swap. If these to process crossing each other, we stop.
	while (1)
	{
		do pright --; while (compare(pright, pstart, param) > 0);
		do pleft  ++; while (compare(pleft, pstart, param) < 0);

		// SWAP
		if (pleft < pright)
		{
			temp = *pleft;
			*pleft = *pright;
			*pright = temp;
		}
		else break;
	}

	// Now all the element lower than the pivot are before the ones greater than the pivot.
	// So, we have 2 groups to sort again. To do that, we are going to use quicksort itself !
	QuickSort(pstart	, pright, comp);
	QuickSort(pright + 1, pend	, comp);
}
*/