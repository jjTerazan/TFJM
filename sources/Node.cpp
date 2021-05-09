
#include "Node.h"

Node::Node(const u8 id, Node* parent)
{
	assert(parent != nullptr);
//	assert(parent->m_Id < 16);
	assert(id < 16);

	m_Id		= id;
	m_deep		= parent->m_deep + 1;
	m_isDead	= parent->m_isDead | (1 << id);
	m_pChild	= nullptr;
	m_pParent	= parent;

	// Insertion en tête de liste ( le dernier 'né' devient le premier de la liste des fils ... c'est la vie ;)
	m_pNxt		= m_pParent->m_pChild;
	m_pParent->m_pChild = this;
}

Node::~Node()
{
	if (m_pChild)
		delete(m_pChild);
	if (m_pNxt)
		delete(m_pNxt);
}

u32 Node::BuildLines(Line* pline, const u8 last)
{
	static u32 total = 0;

	u32 nb = 0;
	// ce node est une extrémité on remonte jusqu'en haut pour construire
	if (m_deep == PLAYERNB - 2)
	{
		assert(!m_pChild);

		Line* pch = pline->m_pChild;
		Line *pl = new Line(pline);

		pl->m_cell[PLAYERNB - 1] = last;
		u8* pc = &pl->m_cell[PLAYERNB - 2];
		Node* p = this;
		while (p)
		{
			*pc = p->m_Id;
			pc--;
			p = p->m_pParent;
		}
		pl->setupChunksAndPinLine();

		total += 1;
		nb = 1;
		
		if (pl->IsValid())
		{
			total += 1;
			nb = 1;
		}
		else
		{
			pline->m_pChild = pl->m_pNxt; // moche mais rapide
			pl->m_pParent = nullptr;
			pl->m_pNxt = nullptr;
			assert(!pl->m_pChild);
			delete pl;
		}
		

		// mon frere
		if (m_pNxt)
			nb += m_pNxt->BuildLines(pline, last);

		//delete this;
		
	}
	else
	{
		// Mon fils
		if (m_pChild)
			nb += m_pChild->BuildLines(pline, last);

		// mon frere
		if (m_pNxt)
			nb += m_pNxt->BuildLines(pline, last);
	
	}
	return nb;
}

void Node::Print()
{
	if (m_deep)
	{
		for (u8 i = 0; i < m_deep; i++)
			printf("\t");
	}

	printf("+%d[%d]\n", m_Id, m_deep);

	// Mon fils
	if (m_pChild)
		m_pChild->Print();

	// mon frere
	if (m_pNxt)
		m_pNxt->Print();

}

static u32 _count(u32 c)
{
	return c + 1;
}
u32 Node::GetLeafNb(u8 deep)
{
	u32 nb = 0;
	if (deep == m_deep)
		nb = 1;
	if (m_pNxt)
		nb += m_pNxt->GetLeafNb(deep);
	if (m_pChild)
		nb += m_pChild->GetLeafNb(deep);
	return nb;
}

