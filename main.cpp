#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <tchar.h>
#include <vector>
#include <stdio.h>
#include <psapi.h>

static TCHAR szWindowClass[] = _T("DesktopApp");
static TCHAR szTitle[] = _T("NFS UNDERGROUND 2");

DWORD Nitro = 0;
DWORD Gear = 0;
DWORD Engine = 0;
DWORD pid = NULL;
HWND winHandle = NULL;
HANDLE hProcess = NULL;
DWORD BaseAddress = 0x0;
DWORD MajorOffset = 0x49CEB0;

std::vector<DWORD> gearOffset = { 0x1E4 };
std::vector<DWORD> nitroOffset = { 0x220, 0x45C };
std::vector<DWORD> engineOffset = { 0x220, 0x440 };

HINSTANCE hInst;
TCHAR WindowName[] = _T("NFS Underground 2");
TCHAR exeName[] = _T("SPEED2.EXE");

void Render(HWND hWnd);
DWORD GetBaseAdress();
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
DWORD ReadByte(DWORD addr, std::vector<DWORD> offset);


int CALLBACK WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("Windows Desktop Guided Tour"),
            NULL);

        return 1;
    }

    hInst = hInstance;

    HWND hWnd = CreateWindow(
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        400, 300,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hWnd)
    {
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("Windows Desktop Guided Tour"),
            NULL);

        return 1;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;

    winHandle = FindWindow(NULL, WindowName);
    if (!winHandle)
    {
        MessageBox(NULL, _T("FindWindow error"), NULL, NULL);
        return 1;
    }
    GetWindowThreadProcessId(winHandle, &pid);

    hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, pid);
    if (!hProcess)
    {
        MessageBox(winHandle, _T("Call to OpenProcess!"), NULL, NULL);
        return 1;
    }

    BaseAddress = GetBaseAdress();

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
        Render(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        CloseHandle(hProcess);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
}

void Render(HWND hWnd)
{
    Gear = ReadByte(BaseAddress, gearOffset);
    Nitro = ReadByte(BaseAddress, nitroOffset);
    Engine = ReadByte(BaseAddress, engineOffset);

    HDC hdc;
    PAINTSTRUCT ps;
    TCHAR gear_str[9];
    TCHAR nitro_str[256];
    TCHAR engine_str[256];

    swprintf(nitro_str, 256, L"Nitro: %u", Nitro);
    if (Gear == 0)
    {
        swprintf(gear_str, 9, L"Gear: R");
    }
    else if (Gear == 1)
    {
        swprintf(gear_str, 9, L"Gear: N");
    }
    else
    {
        swprintf(gear_str, 9, L"Gear: %u ", Gear - 1);
    }
    swprintf(engine_str, 256, L"Engine: %1.f      ", *((float *)&Engine));

    hdc = BeginPaint(hWnd, &ps);
    TextOut(hdc, 10, 30, gear_str, _tcslen(gear_str));
    TextOut(hdc, 10, 10, nitro_str, _tcslen(nitro_str));
    TextOut(hdc, 10, 50, engine_str, _tcslen(engine_str));
    EndPaint(hWnd, &ps);
    InvalidateRect(hWnd, NULL, NULL);
    UpdateWindow(hWnd);
}

DWORD GetBaseAdress()
{
    DWORD Base = 0x0;
    HMODULE hMods[1024];

    if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &pid) == 0)
    {
        MessageBox(winHandle, _T("EnumpPocessModules failed!"), _T("EnumpPocessModules failed!"), NULL);
        return 0;
    }
    else
    {
        for (int i = 0; i < (pid / sizeof(HMODULE)); i++)
        {
            TCHAR szModName[MAX_PATH];
            if (GetModuleFileNameEx(hProcess, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR)))
            {
                if (wcsstr(szModName, exeName))
                {
                    Base = (DWORD)hMods[i];
                }
            }
        }
    }

    return Base;
}

DWORD ReadByte(DWORD addr, std::vector<DWORD> offset)
{
    DWORD value = 0x0;

    if (ReadProcessMemory(hProcess, (LPVOID)(BaseAddress + MajorOffset), &value, sizeof(DWORD), NULL))
    {
        for (int i = 0; i < offset.size(); i++)
        {
            ReadProcessMemory(hProcess, (LPVOID)(value + offset[i]), &value, sizeof(DWORD), NULL);
        }
    }

    return value;
}
