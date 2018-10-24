#pragma once

#include<cstddef>
#include<iostream>
#include"myConstruct.h"
using namespace std;

template<class T>
class myVector{
public:
	typedef T value_type;//元素类型  起别名
	typedef value_type* pointer;//zhizheng
	typedef value_type* iterator;//-----------迭代器
	typedef value_type& reference;//引用
	typedef const value_type* const_pointer;//常量指针
	typedef const value_type& const_reference;//常量引用
	typedef size_t size_type;//数据类型基本尺寸大小
private:
	iterator start;
	iterator finish;
	iterator end_of_storage;
protected:
	void __allocate_add_fill(size_type n, const T& value){//分配空间并填充初始值, 不返回任何值
		iterator result = (iterator)malloc(n*sizeof(T));
		if (result){//result!=0  申请内存成功，在得到的内存上创建对象
			start = result;
			end_of_storage = start + n;
			finish = end_of_storage;
			while (n--){
				construct(result, value);//在内存上，一个个的进行构造对象
				++result;
			}
		}
		else{
			cout << "内存不足，程序终止！" << endl;
			exit(0);
		}
	}

	//分配空间 从first开始复制n个值到新空间中, 并返回新开辟的空间的首地址
	iterator __allocate_and_copy(iterator first, size_type n){
		iterator result = (iterator)malloc(n*sizeof(T));
		iterator _start = result;
		if (0 != result){
			while (n--){
				construct(result, *first);
				++result;
				++first;
			}
			cout << endl;
		}
		else{
			cout << "内存不足，程序终止！" << endl;
			exit(0);
		}
		return _start;
	}

	//将first到last迭代器之间(first,last)的元素拷贝到_start开始的内存中, 并返回 指向 拷贝完所有数据之后最后一个数据的下一个位置的指针
	iterator __copy(iterator first, iterator last, iterator _start){
		while (first < last){
			*_start++ = *first++;
		}
		return _start;
	}

	iterator __fill(iterator first, iterator last, const T& value){
		while (first < last){
			*first++ = value;
		}
		return first;
	}

	//自己写的 从迭代器first开始填充n个值为value的元素
	iterator __fill_n(iterator first, size_type n, const T& value){
		while (n--){
			*first++ = value;
		}
		return first;
	}


	//自己写的  将从 [first,last)所有元素 一一依次后移， 最后的一个元素移到end的位置
	void __backCopy(iterator first, iterator last, iterator end){
		while (first <= last){
			*end-- = *last--;
		}
	}

	

public:
	//返回首元素指针
	iterator begin(){ return start; }
	const iterator begin() const { return start; }

	//返回尾元素下一个位置的指针
	iterator end(){ return finish; }
	const iterator end() const{ return finish; }

	//------------------
	//取元素  自己写
	T front(){ return *begin(); }//取vector中第一个元素
	T back(){ return *(end() - 1); }//取vector中最后一个元素

	//容器大小
	size_type size() const{ return (size_type)(end() - begin()); }

	//容器的实际大小
	size_type capacity() const{ return (end_of_storage - begin()); }

	//判断容器是否为空
	bool empty(){ return begin() == end(); }

	//默认构造函数
	myVector() :start(NULL), finish(NULL), end_of_storage(NULL){ cout << "默认构造函数，不分配空间" << endl; }
	//构造函数重载
	myVector(size_type n, const T& value){ __allocate_add_fill(n, value); }
	myVector(int n, const T&value){ __allocate_add_fill(n, value); }
	myVector(long n, const T&value){ __allocate_add_fill(n, value); }

	myVector(size_type n){ __allocate_add_fill(n, T()); }//??? T()??
	myVector(int n){ __allocate_add_fill(n, T()); }
	myVector(long n){ __allocate_add_fill(n, T()); }

	//拷贝构造函数
	myVector(const myVector<T>& v){
		start = __allocate_add_copy(v.begin(), v.end(), v.end() - v.begin());
		finish = start + (v.end() - v.begin());
		end_of_storage = finish;
	}

	//C c2(first,last);
	myVector(const iterator& first, const iterator& last){
		start = __allocate_add_copy(first, last, size_type(last - first + 1));
		finish = start + (last - first + 1);
		end_of_storage = finish;
	}

	//元素操作
	//删除最后一个元素
	void pop_back(){
		if (!empty()){
			--finish;
			destroy(finish);
		}
	}

	//删除指定位置上的元素，并返回指向删除元素的迭代器
	iterator erase(iterator position){
		if (position > begin() && position < end()){
			__copy(position + 1, finish, position);
		}
		--finish;
		destroy(finish);
		return position;
	}

	//删除指定范围的元素  并返回 迭代器返回的元素
	iterator erase(iterator first, iterator last){
		if (first > begin() && first < last && last < end()){
			iterator iter = __copy(last, finish, first);
			destroy(iter, finish);
			finish -= (last - first);
			return first;
		}
	}

	//清除全部元素
	void clear(){
		erase(begin(), end());
	}

	//在vector容器的后面添加一个元素，值为value
	void push_back(const T& value){
		if (finish != end_of_storage){//如果新加入元素后，空间够用的话
			construct(finish, value);
			++finish;
		}
		else{//如果新加入元素之后 空间不够用的话
			//重新分配空间
			const size_type old_size = size();
			const size_type new_size = (old_size == 0) ? 1 : 2 * old_size;//如果原空间为0，则配置1的大小，如果原空间不为0，则配置原空间二倍大小的新空间到新的地址
			iterator new_start = (iterator)malloc(new_size*sizeof(T));
			iterator new_finish = new_start;//开始时，未拷贝前，结束位置等于起始位置
			iterator new_capacity = new_start + new_size;

			//内存分配要具有原子性，即： 要么全部成功，要么全部失败
			try{//1:将原空间的全部元素拷贝到新空间 2:为新的元素设定初始值 3:调整 new_finish
				for (iterator it = begin(); it < end(); ++it){//1:将原空间的全部元素拷贝到新空间
					construct(new_finish, *it);
				}
				construct(new_finish, value);//2:为新的元素设定初始值
				++new_finish;//3:调整 new_finish
			}
			catch (...){//如果内存分配失败了
				destroy(new_start, new_finish);
				free(new_start);//删除申请到的内存
				new_start = NULL;
				new_finish = NULL;
				throw("重新分配内存失败！");
			}

			destroy(begin(), end());//析构，并释放原vector
			free(start);//删除原vector的内存
			start = new_start;//更改迭代器，指向新的vector
			finish = new_finish;
			end_of_storage = new_start + new_size;
		}

	}

	//插入 在值为value的一个元素到position的位置上
	void insert(iterator position, const T& value){
		insert(position, 1, value);
	}

	//在position位置之后，插入n的值为value的元素
	void insert(iterator position, size_type n, const T& value){
		if (n == 0)return;

		if ((end_of_storage - finish) >= n){//备用空间够插入n个新元素
			T x_copy = value;
			const  size_type size_from_position_to_end = finish - position;

			iterator old_finish = finish;
			if (size_from_position_to_end > n){
				__copy(finish - n, finish, finish);
				finish += n;
				__backCopy(position, old_finish - n, old_finish);
				__fill(position, position + n, x_copy);
			}
			else{
				__fill_n(finish, n - size_from_position_to_end, x_copy);
				finish += n - size_from_position_to_end;
				__copy(position, old_finish, finish);
				finish += size_from_position_to_end;
				__fill(position, old_finish, x_copy);
			}
		}
		else{
			//重新申请空间
			const size_type old_size = size();
			size_type _max = 0;
			if (old_size > n) _max = old_size;
			else _max = n;
			const size_type len = old_size + _max;
			iterator new_start = (iterator)malloc(len * sizeof(T));
			iterator new_finish = new_start;
			//内存的分配要有原子性，即:要么全部成功，要么全部失败。
			try{
				new_finish = __copy(begin(), position, new_start);//1.将原内容 至position的所有元素(不包含position) 拷贝到新的vector
				new_finish = __fill_n(new_finish, n, value);//2.将position位置到后面的n个元素都填充为value
				new_finish = __copy(position, end(), new_finish);//3.拷贝从 position位置到end()位置的原vector的所有剩余元素
			}
			catch (...)//如果失败了
			{
				destroy(new_start, new_finish);
				free(new_start);//删除申请到的内存
				new_start = new_finish = NULL;
				throw;        //抛出异常
			}
			//析构并释放原vector
			destroy(begin(), end());
			//删除内存
			free(start);
			//调整迭代器，指向新的vector
			start = new_start;
			finish = new_finish;
			end_of_storage = new_start + len;
		}
	}

	//重载操作符
	reference operator[](size_type n){ return *(begin() + n); }//取 第n个元素
	const_reference operator[](size_type n) const{ return *(begin() + n); }

	bool operator==(const myVector&v){
		if (v.size() != size())return false;
		iterator it;
		for (it = v.begin(); it < v.end(); ++it){
			if (*it != *(begin() + (it - v.begin()))) break;
		}
		if (it == v.end()) return true;
		else return false;
	}

	bool operator!=(const myVector&v){
		return !(operator==(v));
	}
};
