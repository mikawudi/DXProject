#include<Windows.h>
#include<d3d10.h>
#include "App.h"
#include "BaseDX.h"
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR cmdLine, int nShow)
{
	string str("dx10Class");
	App app(hInstance, str);
	if (!app.AppRegisterClassEx(WndProc))
		return 0;
	if (!app.CreateWindows())
		return 0;
	HWND hwnd = app.GetHwnd();
	BaseDX* dxBase = new BaseDX();
	dxBase->InitD3d(hwnd);
	app.ShowWindows(nShow);
	app.LoopStart(dxBase);
	delete dxBase;
	return 0;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;
	switch (message)
	{
	case WM_PAINT:
		hDC = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}