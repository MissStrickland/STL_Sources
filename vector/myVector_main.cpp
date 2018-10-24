#include"myVector.h"

void disp(const myVector<int> &v){
	for (myVector<int>::iterator it = v.begin(); it != v.end(); ++it){
		cout << *it << " ";
	}
	cout << endl;
}
void test(){
	cout << "test--------" << endl;
	myVector<int> v(100);
	v.push_back(10);
	v.push_back(9);
	v.push_back(8);
	v.push_back(7);
	v.push_back(6);
	v.push_back(5);
	for (int i = 0; i < v.size(); ++i)
		cout << v[i] << " ";
	cout << endl;

	myVector<int>::iterator it;
	for (it = v.begin(); it != v.end(); ++it){
		cout << *it << " ";
	}
	cout << endl;
	cout << "size: " << v.size() << endl;
}

void test1(){
	cout << "test1--------" << endl;
	myVector<int> v(100);
	v.push_back(10);
	v.push_back(9);
	v.push_back(8);
	v.push_back(7);
	v.push_back(6);
	v.push_back(5);

	myVector<int>::iterator it = v.begin() + 3;

	v.insert(it, 3, 300); disp(v);
	v.insert(it, 4, 500); disp(v);
	v.insert(it, 2, 200); disp(v);
	v.insert(it, 2, 20); disp(v);
	v.insert(it, 3, 30); disp(v);
	v.insert(it, 4, 40); disp(v);
}



void test2(){
	cout << "test2--------" << endl;
	myVector<int> v(100);
	v.push_back(10);
	v.push_back(9);
	v.push_back(8);
	v.push_back(7);
	v.push_back(6);
	v.push_back(5);

	myVector<int>::iterator it = v.begin();

	v.insert(it, 300); disp(v);
	v.insert(it, 500); disp(v);
	v.insert(it, 200); disp(v);
	v.insert(it, 20); disp(v);
	v.insert(it, 30); disp(v);
	v.insert(it, 40); disp(v);

}
int main(){
	test();
	test1();
	test2();

	return 0;
}

/*
test--------
10 9 8 7 6 5
10 9 8 7 6 5
size: 6
test1--------
10 9 300 300 300 8 7 6 5
10 9 500 500 500 500 300 300 300 8 7 6 5
10 9 200 200 500 500 500 500 300 300 300 8 7 6 5
10 9 20 20 200 200 500 500 500 500 300 300 300 8 7 6 5
10 9 30 30 30 20 20 200 200 500 500 500 500 300 300 300 8 7 6 5
10 9 40 40 40 40 30 30 30 20 20 200 200 500 500 500 500 300 300 300 8 7 6 5
test2--------
10 9 300 8 7 6 5
10 9 500 300 8 7 6 5
10 9 200 500 300 8 7 6 5
10 9 20 200 500 300 8 7 6 5
10 9 30 20 200 500 300 8 7 6 5
10 9 40 30 20 200 500 300 8 7 6 5
请按任意键继续. . .
*/
/*
该insert
不能在结尾插入，结尾插入就成为了在原vector的固定末尾位置插入新元素，并将之前插入的元素向后挤了挤
test--------
10 9 8 7 6 5
10 9 8 7 6 5
size: 6
test1--------
10 9 8 7 6 5 300 300 300
10 9 8 7 6 5 500 500 500 500 300 300 300
10 9 8 7 6 5 200 200 500 500 500 500 300 300 300
10 9 8 7 6 5 20 20 200 200 500 500 500 500 300 300 300
10 9 8 7 6 5 30 30 30 20 20 200 200 500 500 500 500 300 300 300
10 9 8 7 6 5 40 40 40 40 30 30 30 20 20 200 200 500 500 500 500 300 300 300
test2--------
10 9 8 7 6 5 300
10 9 8 7 6 5 500 300
10 9 8 7 6 5 200 500 300
10 9 8 7 6 5 20 200 500 300
10 9 8 7 6 5 30 20 200 500 300
10 9 8 7 6 5 40 30 20 200 500 300
请按任意键继续. . .
*/
