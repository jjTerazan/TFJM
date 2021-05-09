#pragma once
#include "TFJM.h"

template<typename T>
class Pile
{
	public:
	 Pile(const u32 sz = 1024) { m_pile = (T**)malloc(sizeof(T*) * sz); m_w = m_pile; m_wx = m_pile + sz; }
	~Pile() { free(m_pile); }

	inline T*	pop() {if (m_w == m_pile)return nullptr; else { m_w--; return *m_w; }}
	inline u32	isEmpty() { return (m_w == m_pile); }
	inline u32	size() { return (m_w - m_pile); }
	inline u32	capacity() { return (m_wx - m_pile); }
	void		push(T* p); // implementé dans "Pile.tpp"
	inline void resize0() { m_w = m_pile; }

	T**	m_pile;	
	T**	m_w;
	T**	m_wx;
};

#include "Pile.tpp"
