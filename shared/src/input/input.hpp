#pragma once

namespace shared::input {
    inline void simulate_key(int virtual_key) {
        INPUT input{};
        memset(&input, 0, sizeof(INPUT));
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = virtual_key;
        input.ki.dwFlags = 0;

        SendInput(1, &input, sizeof(INPUT));

        input.ki.dwFlags = KEYEVENTF_KEYUP;

        SendInput(1, &input, sizeof(INPUT));
    }

    inline void simulate_click(std::pair<int, int> position) {
        SetCursorPos(position.first, position.second);

        INPUT input{};
        memset(&input, 0, sizeof(INPUT));
        input.type = INPUT_MOUSE;
        input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

        SendInput(1, &input, sizeof(INPUT));

        input.mi.dwFlags = MOUSEEVENTF_LEFTUP;

        SendInput(1, &input, sizeof(INPUT));
    }
}