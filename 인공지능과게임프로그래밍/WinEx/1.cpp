#include <windows.h>
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	if(MessageBox(NULL, lpCmdLine, "Title", MB_OKCANCEL) == IDOK) {
		MessageBox(NULL, lpCmdLine, "Ok", MB_OKCANCEL);
	}

	return 0;
}