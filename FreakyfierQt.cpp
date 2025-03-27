#include "FreakyfierQt.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>
#include <QLabel>
#include <QGraphicsDropShadowEffect>
#include <QPainterPath>
#include <windows.h>

HHOOK hHook = NULL;

char32_t to_math_bold(wchar_t c) {
    if (c >= L'A' && c <= L'Z') {
        return c + 0x1D4D0 - L'A';
    }
    else if (c >= L'a' && c <= L'z') {
        return c + 0x1D4EA - L'a';
    }
    else if (c >= L'0' && c <= L'9') {
        return c + 0x1D7CE - L'0';
    }
    return c;
}

void sendSurrogatePair(char32_t target) {
    wchar_t high_surrogate = 0xD800 + ((target - 0x10000) >> 10);
    wchar_t low_surrogate = 0xDC00 + ((target - 0x10000) & 0x3FF);

    std::vector<INPUT> in(4);
    ZeroMemory(&in[0], in.size() * sizeof(INPUT));

    in[0].type = INPUT_KEYBOARD;
    in[0].ki.wVk = 0;
    in[0].ki.wScan = high_surrogate;
    in[0].ki.dwFlags = KEYEVENTF_UNICODE;

    in[1].type = INPUT_KEYBOARD;
    in[1].ki.wVk = 0;
    in[1].ki.wScan = low_surrogate;
    in[1].ki.dwFlags = KEYEVENTF_UNICODE;

    in[2] = in[0];
    in[2].ki.dwFlags |= KEYEVENTF_KEYUP;

    in[3] = in[1];
    in[3].ki.dwFlags |= KEYEVENTF_KEYUP;

    SendInput(in.size(), &in[0], sizeof(INPUT));
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
        KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;
        DWORD vkCode = p->vkCode;

        WCHAR buffer[4] = { 0 };
        BYTE originalKeyState[256];
        GetKeyboardState(originalKeyState);

        BYTE keyState[256];
        memcpy(keyState, originalKeyState, 256);

        keyState[VK_SHIFT] = (GetKeyState(VK_SHIFT) & 0x8000) ? 0x80 : 0x00;
        keyState[VK_CAPITAL] = (GetKeyState(VK_CAPITAL) & 0x0001) ? 0x01 : 0x00;

        int result = ToUnicode(vkCode, p->scanCode, keyState, buffer, sizeof(buffer) / sizeof(WCHAR), 0);

        if (result > 0) {
            wchar_t typedChar = buffer[0];
            char32_t boldChar = to_math_bold(typedChar);

            if (boldChar != typedChar) {
                if (boldChar >= 0x10000) {
                    sendSurrogatePair(boldChar);
                }
                else {
                    INPUT input;
                    input.type = INPUT_KEYBOARD;
                    input.ki.wVk = 0;
                    input.ki.wScan = (WORD)boldChar;
                    input.ki.dwFlags = KEYEVENTF_UNICODE;
                    input.ki.time = 0;
                    input.ki.dwExtraInfo = 0;

                    SendInput(1, &input, sizeof(INPUT));
                    input.ki.dwFlags |= KEYEVENTF_KEYUP;
                    SendInput(1, &input, sizeof(INPUT));
                }

                return 1;
            }
        }
    }
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

void ToggleHook(bool enable) {
    if (enable && !hHook) {
        hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    }
    else if (!enable && hHook) {
        UnhookWindowsHookEx(hHook);
        hHook = NULL;
    }
}

FreakyfierQt::FreakyfierQt(QWidget* parent)
    : QMainWindow(parent), m_isHookEnabled(false)
{
    setupUI();
}

FreakyfierQt::~FreakyfierQt()
{
    if (hHook) {
        UnhookWindowsHookEx(hHook);
    }
}

void FreakyfierQt::setupUI()
{
    setWindowTitle("Freakyfier");
    setFixedSize(400, 300);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);

    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    QLabel* titleLabel = new QLabel("Freakyfier", this);
    titleLabel->setStyleSheet(R"(
        QLabel {
            font-size: 24px;
            font-weight: bold;
            color: #2c3e50;
            margin-bottom: 10px;
        }
    )");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    QLabel* subtitleLabel = new QLabel("𝓯𝓻𝓮𝓪𝓴𝓲𝓯𝔂 your text!", this);
    subtitleLabel->setStyleSheet(R"(
        QLabel {
            font-size: 14px;
            color: #7f8c8d;
            margin-bottom: 20px;
        }
    )");
    subtitleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(subtitleLabel);

    m_statusLabel = new QLabel("Status: Disabled", this);
    m_statusLabel->setStyleSheet(R"(
        QLabel {
            font-size: 16px;
            font-weight: bold;
            padding: 10px;
            border-radius: 5px;
            background-color: #e74c3c;
            color: white;
        }
    )");
    m_statusLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_statusLabel);

    m_toggleButton = new QPushButton("Activate", this);
    m_toggleButton->setStyleSheet(R"(
        QPushButton {
            background-color: #3498db;
            color: white;
            border: none;
            padding: 12px 24px;
            font-size: 16px;
            border-radius: 6px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #2980b9;
        }
        QPushButton:pressed {
            background-color: #21618c;
        }
    )");
    connect(m_toggleButton, &QPushButton::clicked, this, &FreakyfierQt::toggleHook);
    mainLayout->addWidget(m_toggleButton);

    QLabel* infoLabel = new QLabel("Click the button to enable/disable text conversion", this);
    infoLabel->setStyleSheet(R"(
        QLabel {
            color: #95a5a6;
            font-size: 12px;
        }
    )");
    infoLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(infoLabel);

    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(20);
    shadowEffect->setColor(QColor(0, 0, 0, 30));
    shadowEffect->setOffset(0, 4);
    centralWidget->setGraphicsEffect(shadowEffect);

    setCentralWidget(centralWidget);
    setStyleSheet(R"(
        QMainWindow {
            background-color: #ecf0f1;
        }
    )");
}

void FreakyfierQt::toggleHook()
{
    m_isHookEnabled = !m_isHookEnabled;

    if (m_isHookEnabled) {
        m_statusLabel->setText("Status: Enabled");
        m_statusLabel->setStyleSheet(R"(
            QLabel {
                font-size: 16px;
                font-weight: bold;
                padding: 10px;
                border-radius: 5px;
                background-color: #2ecc71;
                color: white;
            }
        )");
        m_toggleButton->setText("Deactivate");
    }
    else {
        m_statusLabel->setText("Status: Disabled");
        m_statusLabel->setStyleSheet(R"(
            QLabel {
                font-size: 16px;
                font-weight: bold;
                padding: 10px;
                border-radius: 5px;
                background-color: #e74c3c;
                color: white;
            }
        )");
        m_toggleButton->setText("Activate");
    }

    ToggleHook(m_isHookEnabled);
}