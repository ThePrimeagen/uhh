#pragma once

#include <memory>
#include <string>

enum class CursorDirection {
    Up,
    Down,
    Forward,
    Back
};

struct Size {
    int rows;
    int columns;
};

struct TerminalImpl;

class Terminal {

  public:
    enum Key {
        CTRL_A = 1,
        CTRL_B,
        CTRL_C,
        CTRL_D,
        CTRL_E,
        CTRL_F,
        CTRL_G,
        CTRL_H,
        BACKSPACE = CTRL_H,
        CTRL_I,
        TAB = CTRL_I,
        CTRL_J,
        ENTER = CTRL_J,
        CTRL_K,
        CTRL_L,
        CTRL_M,
        LF = CTRL_M,
        CTRL_N,
        CTRL_O,
        CTRL_P,
        CTRL_Q,
        CTRL_R,
        CTRL_T,
        CTRL_S,
        CTRL_U,
        CTRL_V,
        CTRL_W,
        CTRL_X,
        CTRL_Y,
        CTRL_Z,
        ESC,
        DEL = 127
    };

    Terminal();
    ~Terminal();

    Terminal(const Terminal&) = delete;
    
    Size get_screen_size();
    
    void putString(const std::string& str);
    void putHighlighted(const std::string& str);
    
    void putChar(char ch);
    char getChar();
    int getKeyCode();

    void moveCursor(CursorDirection direction, int n);
    void moveToColumn(int n);
    
    void eraseDisplay();
    void eraseLine();

  private:
    std::unique_ptr<TerminalImpl> impl;
};