#pragma once
#include <assert.h>
template<typename T>
class Singleton
{
public:
	static T* InstancePtr() {
		assert(m_singleton);
		return m_singleton;
	}
	~Singleton() {
		assert(m_singleton);
		m_singleton = nullptr;
	}
	Singleton() {
		assert(!m_singleton);
		m_singleton = static_cast<T*>(this);
	}

protected:
	static T* m_singleton;
};


#define SINGLETON_INIT(TYPE)\
TYPE* Singleton<TYPE>::m_singleton = nullptr;