
#include "NodePile.h"

void NodePile::push(Node* p)
{
	if (m_w == m_wx)
	{
		u32 sz = m_wx - m_pile;
		// on double la taille
		m_pile = (Node**)realloc(m_pile, sizeof(Node*) * sz * 2);
		m_wx = m_pile + sz * 2;
		m_w = m_pile + sz;
	}

	*m_w = p;
 	 m_w++;
}

