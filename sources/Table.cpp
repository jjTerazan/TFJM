#include "TFJM.h"
#include "Line.h"
#include "Table.h"

//#include "QuickSortTemplate.h"

Table::Table()
{
	memset(m_pLine, 0, sizeof(Line*) * PLAYERNB);
}

Table::~Table()
{
}

void Table::write(FILE* pfile)
{
	for (u8 i = 0; i < PLAYERNB; i++)
	{
		m_pLine[i]->write(pfile);
		fprintf(pfile, "\n");
	}
	fprintf(pfile, "\n");
}

void Table::read(FILE* pfile)
{
	// TODO avec fgets
}

void Table::Print()
{
	for (u8 i = 0; i < PLAYERNB; i++)
	{
		m_pLine[i]->Print();
		printf("\n");
	}
}

typedef s32(*QSCOMPARE)(const Line* pa, const Line* pb, const u32 param); // return values: pa<pb return -1, pa>pb return 1, pa==pb return 0   
static s32 _linecomp(const Line* pa, const Line* pb, const u32 param)
{
	if (pa->m_PinLine[param] > pb->m_PinLine[param])
		return 1;
	else if (pa->m_PinLine[param] == pb->m_PinLine[param])
		return 0;
	else // pa->m_PinLine[param] < pb->m_PinLine[param]
		return -1;
}

void _QSort(Line** pstart, Line** pend, const u32 param)
{
	Line* temp;
	Line** pleft = pstart - 1;
	Line** pright = pend + 1;

	// Rien A Faire Si la taille du buffer est nulle
	if (pstart >= pend)
		return;

	// Otherwise, parsing array one time from right to left, and another time from left to right
	// to look for some miss placed element which swap. If these to process crossing each other, we stop.
	while (1)
	{
		do pright--; while (_linecomp(*pright, *pstart, param) > 0);
		do pleft++; while (_linecomp(*pleft, *pstart, param) < 0);

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
	_QSort(pstart, pright, param);
	_QSort(pright + 1, pend, param);
}

void Table::QSort(const u8 sz, const u8 id)
{
	_QSort(m_pLine,&m_pLine[sz-1], id);
}

bool Table::IsValid(const u8 sz)
{
	u8 i, j,k;
	for (i = 0; i < sz; i++)
	{
		QSort(sz,i);
		// Test sur la ligne 0 ( le premier test est toujours valide donc on fait juste le second )
		for (k = 1; k < sz; k++)
		{
			if ((m_pLine[0]->m_chunkR[i] & (m_pLine[k]->m_chunkL[i] | m_pLine[k]->m_chunkR[i])) == 0)
				return false;
		}
		// Test sur les lignes suivantes:
		for (j = 1; j < sz-1; j++)
		{
			for (k = j + 1; k < sz; k++)
			{
				if((m_pLine[j]->m_chunkL[i]& m_pLine[k]->m_chunkL[i]) == m_pLine[j]->m_chunkL[i])
					if ((m_pLine[j]->m_chunkR[i] & (m_pLine[k]->m_chunkL[i] | m_pLine[k]->m_chunkR[i])) == 0)
						return false;
			}
		}
	}
	return true;
}

