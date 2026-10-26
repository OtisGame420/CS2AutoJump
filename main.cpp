
#include <windows.h>
#include <iostream>

#pragma comment(lib, "User32.lib")

HHOOK mouseHook = nullptr;

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION && wParam == WM_MOUSEWHEEL)
    {
        MSLLHOOKSTRUCT* mouse =
            reinterpret_cast<MSLLHOOKSTRUCT*>(lParam);

        short delta = GET_WHEEL_DELTA_WPARAM(mouse->mouseData);

        if (delta != 0)
        {
            INPUT input{};
            input.type = INPUT_KEYBOARD;
            input.ki.wVk = VK_SPACE;

            SendInput(1, &input, sizeof(INPUT));

            input.ki.dwFlags = KEYEVENTF_KEYUP;
            SendInput(1, &input, sizeof(INPUT));
        }
    }

    return CallNextHookEx(mouseHook, nCode, wParam, lParam);
}

int main()
{
    std::cout << "CS2 ScrollJump uruchomiony!\n";
    std::cout << "Przewijaj scroll, aby wyslac skok.\n";
    std::cout << "F10 = wyjscie\n";

    mouseHook = SetWindowsHookExW(
        WH_MOUSE_LL,
        MouseProc,
        GetModuleHandleW(nullptr),
        0
    );

    if (!mouseHook)
    {
        std::cerr << "Nie udalo sie uruchomic hooka myszy.\n";
        std::cerr << "Blad Windows: " << GetLastError() << "\n";
        return 1;
    }

    MSG msg{};

    while (GetMessageW(&msg, nullptr, 0, 0) > 0)
    {
        if (GetAsyncKeyState(VK_F10) & 1)
            break;

        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    UnhookWindowsHookEx(mouseHook);
    return 0;
}
