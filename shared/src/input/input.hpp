#pragma once

namespace shared::input
{
    void simulate_key(int virtual_key)
    {
        keybd_event(virtual_key, 0, 0, 0);
        keybd_event(virtual_key, 0, KEYEVENTF_KEYUP, 0);
    }

    inline void simulate_click(std::pair<int, int> position)
    {
        SetCursorPos(position.first, position.second);

        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
        mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
    }

    inline void send_text(const std::wstring& text)
    {
        for (const auto& character : text)
        {
            keybd_event(0, character, KEYEVENTF_UNICODE, 0);
            keybd_event(0, character, KEYEVENTF_UNICODE | KEYEVENTF_KEYUP, 0);
        }
    }
}