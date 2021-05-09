#pragma once

#include "TFJM.h"

#include "Line.h"
class Table
{
	public:
	Table();
	~Table();
	void write(FILE* pfile);
	void read(FILE* pfile);
	void Print();
	void QSort(const u8 sz, const u8 id);
	bool IsValid(const u8 sz);

	Line* m_pLine[PLAYERNB];
};
