#pragma once

#include "TFJM.h"
#include "Pile.h"

class Table;
class NodePile;
class Line
{
	public:
	Line(Line* parent = nullptr);
	Line(const u8* pfull, Line* parent = nullptr);
	Line(const u8* pmid, const u8 first, const u8 target, Line* parent=nullptr);
	~Line();
	void setupChunksAndPinLine();
	bool IsValid();
	void Remove();
	u32	 GetChildrenNb();
	void write(FILE* pfile);
	void read(FILE* pfile);
	void Print();
	void DeepPrint();
	void buildChildren(u8* ptarget, NodePile* pile);
	u32	 GetNbLeaf(const u8 deep);
	u32	 GetNbLeaf();
	void Clean();
	u32  ExtractTables(Pile<Table>* ppile);
	
	u16		m_chunkL[PLAYERNB];		// ... situés avant
	u16		m_chunkR[PLAYERNB];		// ... situés après
	u16		m_chunkX[PLAYERNB];		// ... avant & après & parent
	
	u8		m_deep;				// Rang de cette ligne
	u8		m_cell[PLAYERNB];	// La ligne à proprement parlé
	u8		m_PinLine[PLAYERNB];// pour chaque joueur, sa position on a:  m_cell[ m_PinLine[p] ] = p ET m_PinLine[m_cell[c]] = c
	Line*	m_pParent;			// pointe sur le noeud parent
	Line*	m_pChild;			// pointe sur le premier fils
	Line*	m_pNxt;				// pointe sur le prochain frere
};
