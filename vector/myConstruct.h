#pragma once

template<class T1, class T2>
inline void construct(T1 *p, const T2 &value){
	new (p)T1(value);      //用placement new在所指对象上创建一个对象，value是初始化对象的值
}

template<class T>
inline void destroy(T* pointer){//只做了一层包装，将指针所指的对象析构--通过直接调用类的析构函数
	pointer->~T();
}

template<class ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last){//destroy的泛化版 接受两个迭代器为参数
	for (; first < last; ++first){
		destroy(&*first);//得 理解一下  &*
	}
}

inline void destroy(char*, char*){};//针对 char* 的特化版
inline void destroy(wchar_t*, wchar_t*){};//针对 wchar_t* 的特化版
