#pragma once

namespace shared
{
    class c_input
    {
    public:
        static void simulate_key(int virtual_key)
        {
            keybd_event(virtual_key, 0, 0, 0);
            keybd_event(virtual_key, 0, KEYEVENTF_KEYUP, 0);
        }

        static void simulate_click(shared::c_vector2<int> position)
        {
            SetCursorPos(position.x, position.y);

            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
        }
    };
}