#pragma once

#include "TFJM.h"
#include "Line.h"

class Node
{
	public:
	//Node():m_Id(0),m_pParent(nullptr),m_pChild(nullptr),m_pNxt(nullptr){}
		Node(const u8 id = 0) :m_Id(id), m_deep(0),m_isDead(1 << id), m_pParent(nullptr), m_pChild(nullptr), m_pNxt(nullptr) {};
	Node(const u8 id, Node* parent);
	~Node();
	inline u32 IsBefore(Node* pn) { Node* p = m_pParent; while (p) { if (p == pn) { return 1; } else { p = p->m_pParent; } }  return 0;}
	inline u32 IsBefore(u8 id) { Node* p = m_pParent; while (p) { if (p->m_Id == id) { return 1; } else { p = p->m_pParent; } }  return 0; }
	u32 BuildLines(Line* pline, const u8 last);
	void Print();
	u32 GetLeafNb(u8 deep);
	u8		m_Id;		// N° du joueur 
	u8		m_deep;		// Rang de ce noeud ( = position sur la ligne )
	u16		m_isDead;	// champ de 16 bits le bit n correspond au joueur nommé n. Le bit est à 1 si le joueur n est mort avant moi ( et donc est un de mes parents )
						// le bit correspondant à mon id est à 1 car je suis mort.

	Node*	m_pParent;	// pointe sur le noeud parent, c'est à dire la cellule précédente dans la liste de Kills.	
	Node*	m_pChild;	// pointe sur le 'premier fils', c'est à dire la première cellule suivante possible dans la liste de Kills.
	Node*	m_pNxt;		// pointe sur le prochain frere, c'est à dire la prochaine cellule suivante de m_pParent, possible dans la liste de Kills.
};
