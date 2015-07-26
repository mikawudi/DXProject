#include "app.h"
#include <string>
#include "BaseDX.h"
#include "Triangle.h";
#include "Box.h"
using namespace std;
App::App(HINSTANCE& instance, const string& winClassName)
	:_instance(instance), _strWinClassName(winClassName)
{

}
bool App::AppRegisterClassEx(WNDPROC process)
{
	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = process;
	wndClass.hInstance = _instance;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = _strWinClassName.c_str();
	return RegisterClassEx(&wndClass);
}
bool App::CreateWindows()
{
	RECT rc = { 0, 0, 640, 480 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);
	_hwnd = CreateWindow(_strWinClassName.c_str(), "Blank Win32 Window", WS_OVERLAPPEDWINDOW, 0, 0, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, _instance, NULL);
	return _hwnd != 0;
}
void App::ShowWindows(int& nShow)
{
	ShowWindow(_hwnd, nShow);
}
int cc = 0;
void App::LoopStart(BaseDX *baseDx)
{
	float color[] = { 0.0f, 0.0f, 0.25f, 1.0f };
	Triangle* triangle = new Triangle(baseDx->_device);
	//Box* box = new Box();
	//box->init(baseDx->_device, 1.0f);
	//baseDx->_device->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	baseDx->SetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	
	//D3DXCOLOR fontColor = {0.0f, 1.0f, 0.0f, 1.0f};
	//RECT R = { 35, 50, 0, 0 };
	while (_msg.message != WM_QUIT)
	{
		cc += 1;
		if (cc % 15 == 0)
			triangle->ChangeRolY();
		/*color[1] = fmod((color[1] + 0.0002f), 1);
		color[2] = fmod((color[2] + 0.0001f), 1);*/
		baseDx->Clear(color);
		baseDx->ApplyPass("ColorTech", "Pass1");
		baseDx->SetConstFXgWVP("gWVP", triangle->CaculateWorldMatrix());
		triangle->draw();
		//box->draw();
		//baseDx->DrawStr(str, fontColor, R);
		baseDx->FlushTargetView();
		if (PeekMessage(&_msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&_msg);
			DispatchMessage(&_msg);
		}
		else
		{
			//Update
			//Draw
		}
	}
}
HWND App::GetHwnd()
{
	return this->_hwnd;
}
App::~App()
{

}