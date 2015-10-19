#include <windows.h>
#include <tchar.h>
class Cdebugmode
{

public:
	int DebugWindow();
private:
	WNDCLASSEX wc;
	HWND hWnd;
	MSG msg;
};