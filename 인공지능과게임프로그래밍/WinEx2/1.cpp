#include <windows.h>
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine,
	int nCmdShow) {
	WNDCLASSEX windowClass;
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = NULL;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = "WinApp Class";
	windowClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
	if (!RegisterClassEx(&windowClass)) return 0;
	HWND hwnd = CreateWindowEx(
		NULL,
		"WinApp Class",
		"Title of Program",
		WS_OVERLAPPEDWINDOW,
		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, // Parent window 
		NULL, // Menu
		hInstance, // Instance handle
		NULL // Additional application data
	);
	if (hwnd == NULL) return 0;
	ShowWindow(hwnd, SW_SHOW);// nCmdShow);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	HBRUSH NewBrush = (HBRUSH)GetStockObject(GRAY_BRUSH);
	switch (uMsg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, NewBrush);
		Rectangle(hdc, 50, 50, 300, 200);
		RECT rt = { 0, 0, 500, 300 };
		DrawText(hdc, "WinApp", -1, &rt, DT_LEFT);
		EndPaint(hwnd, &ps);
	}
	return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}