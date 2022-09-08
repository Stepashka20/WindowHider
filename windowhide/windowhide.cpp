#include <iostream>
#include <windows.h>
#include "wndhide.h"
#include <string>
#include <vector>
#include <map>

using namespace std;

std::string ws2s(const std::wstring& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, 0, 0);
    std::string r(len, '\0');
    WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, &r[0], len, 0, 0);
    return r;
}
string GetWindowStringText(HWND hwnd)
{
    int len = GetWindowTextLengthW(hwnd) + 1;
    vector<wchar_t> buf(len);
    GetWindowTextW(hwnd, &buf[0], len);
    wstring wide = &buf[0];
    return ws2s(wide);
}


int main() {
    setlocale(LC_ALL, "Russian");

    map<int, HWND> windows;
    int i = 0;
    for (HWND hwnd = GetTopWindow(NULL); hwnd != NULL; hwnd = GetNextWindow(hwnd, GW_HWNDNEXT)) {
        if (!IsWindowVisible(hwnd))
            continue;
        int length = GetWindowTextLength(hwnd);
        if (length == 0)
            continue;
        i++;
        cout << i << " => [" << GetWindowStringText(hwnd) << "]" << endl;
        windows[i] = hwnd;
    }
    int name_input_hwnd;
    cout << "Введите номер окна: ";
    cin >> name_input_hwnd;
    if (!windows[name_input_hwnd]) {
        cout << "Окна не существует!" << endl;
        return 0;
    }
    HWND test_window = windows[name_input_hwnd];
    DWORD pid = 0;
    GetWindowThreadProcessId(test_window, &pid);
    if (!pid) {
        cout << "Произошла ошибка" << endl;
        return 0;
    }

    bool status = wndhide::hide_window(pid, test_window);
    if (status) {
        cout << "Успех" << endl;
    } else {
        cout << "Ошибка" << endl;
    }
    return 0;
    
}
