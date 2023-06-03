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

    void send_text(const std::wstring& text) {
        std::vector<INPUT> inputs{};
        inputs.resize(text.size() * 2);

        for (int i = 0; i < text.size(); ++i) {
            const auto character = text[i];

            inputs[i * 2].type = INPUT_KEYBOARD;
            inputs[i * 2].ki.wVk = 0;
            inputs[i * 2].ki.wScan = character;
            inputs[i * 2].ki.dwFlags = KEYEVENTF_UNICODE;

            inputs[i * 2 + 1] = inputs[i * 2];
            inputs[i * 2 + 1].ki.dwFlags |= KEYEVENTF_KEYUP;
        }

        const auto result = SendInput(static_cast<unsigned int>(inputs.size()), inputs.data(), sizeof(INPUT));
        if (!result) {
            spdlog::error("failed to send text");
        }
    }
}