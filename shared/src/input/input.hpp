#pragma once

namespace shared
{
    class c_input
    {
    public:
        static void simulate_key(const int virtual_key)
        {
            keybd_event(static_cast<uint8_t>(virtual_key), 0, 0, 0);
            keybd_event(static_cast<uint8_t>(virtual_key), 0, KEYEVENTF_KEYUP, 0);
        }

        static void simulate_click(const shared::ivector2& position)
        {
            SetCursorPos(position.m_x, position.m_y);

            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
        }
    };
}
