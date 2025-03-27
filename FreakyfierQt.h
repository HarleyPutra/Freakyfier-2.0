#ifndef FREAKYFIERQT_H
#define FREAKYFIERQT_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <windows.h>
#include <vector>

class FreakyfierQt : public QMainWindow
{
    Q_OBJECT

public:
    explicit FreakyfierQt(QWidget* parent = nullptr);
    ~FreakyfierQt();

    // Static method to get/set the current instance
    static FreakyfierQt* getInstance(FreakyfierQt* instance = nullptr);

private:
    void setupUI();

    // Keyboard hook procedure (static to work with Windows API)
    static LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

    // Utility methods
    char32_t to_math_bold(wchar_t c);
    void sendSurrogatePair(char32_t target);

private slots:
    void toggleHook();

private:
    QLabel* m_statusLabel;
    QPushButton* m_toggleButton;

    bool m_isHookEnabled;
    HHOOK m_hook;
};

#endif // FREAKYFIERQT_H