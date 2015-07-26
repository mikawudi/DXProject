// TestVTable.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <vector>
using namespace std;
class BaseFunc
{
public:
	int testInt;
	BaseFunc();
	BaseFunc(const BaseFunc& bfun);
	virtual void TestFunc();
	virtual void TestFunc2();
};

BaseFunc::BaseFunc(const BaseFunc& bfun)
{
	this->testInt = bfun.testInt;
	cout << "this is cotor(BaseFunc&)" << endl;
}
BaseFunc::BaseFunc()
	:testInt(10)
{
	cout << "tis is coter" << endl;
}
void BaseFunc::TestFunc()
{
	cout << "this is Base::TestFunc" << endl;
}
void BaseFunc::TestFunc2()
{
	cout << "this is Base::TestFunc2" << endl;
}

int _tmain(int argc, _TCHAR* argv[])
{

	int aaa = sizeof(int);
	cout << aaa << endl;
	vector<BaseFunc> test{ 10, BaseFunc() };
	BaseFunc bFun;
	BaseFunc *pBFun = &bFun;
	int *iPoint = (int*)pBFun;
	void(*MyFunPoint)();
	MyFunPoint = (void(*)())(*(int*)*iPoint);
	int* tes = ((int*)*iPoint);
	tes++;
	void(*MyFunPoint2)() = (void(*)())*tes;
	cout << sizeof(MyFunPoint) << endl;
	MyFunPoint();
	MyFunPoint2();
	cout << *iPoint << endl;
	return 0;
}

