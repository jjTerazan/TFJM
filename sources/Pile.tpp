template<typename T>
void	 Pile<T>::push(T* p)
{
	if (m_w == m_wx)
	{
		u32 sz = m_wx - m_pile;
		m_pile = (T**)realloc(m_pile, sizeof(T*) * sz * 2); // size multipliée par 2 
		m_wx = m_pile + sz * 2;
		m_w = m_pile + sz;
	}
	*m_w = p;
	m_w++;
}
