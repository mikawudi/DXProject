#include <windows.h>
#include <string>
#include "BaseDX.h"
using namespace std;
#ifndef APP_H_
#define APP_H_
class App
{
public:
	App(HINSTANCE& instance, const string& winClassName);
	virtual ~App();
	virtual bool AppRegisterClassEx(WNDPROC process);
	virtual bool CreateWindows();
	virtual void ShowWindows(int& nShow);
	virtual void LoopStart(BaseDX *baseDx);
	HWND GetHwnd();
private:
	const HINSTANCE& _instance;
	const string& _strWinClassName;
	HWND _hwnd;
	MSG _msg;
	
protected:
};
#endif