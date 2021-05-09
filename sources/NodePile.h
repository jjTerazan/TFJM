#pragma once
#include "TFJM.h"
#include "Node.h"
class NodePile
{
	public:
		NodePile(const u32 sz = 1024) { m_pile = (Node**)malloc(sizeof(Node*) * sz); m_w = m_pile; m_wx = m_pile + sz; }
	~NodePile() { free(m_pile); }

	inline Node* pop() {if (m_w == m_pile)return nullptr; else { m_w--; return *m_w; }}
	void	 push(Node* p);
	u32		 isEmpty() { return (m_w == m_pile); }
	Node**	m_pile;	
	Node**	m_w;
	Node**	m_wx;
};
