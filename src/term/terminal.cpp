#include "term/terminal.h"

#include <stdexcept>

extern "C" {
#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
}

const char* TERMINAL = "/dev/tty";

struct TerminalImpl {
    FILE *tty;
    int fd;
    struct termios oldTermios, newTermios;
};

Terminal::Terminal() {
    FILE* tty = fopen(TERMINAL, "r+");
    if (!tty) {
        throw std::invalid_argument("could not open tty");
    }
    setbuf(tty, nullptr);
    int fd = fileno(tty);
    termios existingTermios;
    if (tcgetattr(fd, &existingTermios) < 0) {
        throw std::runtime_error("unable to read terminal parameters");
    }

    termios newTermios = existingTermios;
    newTermios.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    if (tcsetattr(fd, TCSAFLUSH, &newTermios) < 0) {
        throw std::runtime_error("failed to set terminal parameters");
    }
    this->impl = std::make_unique<TerminalImpl>(TerminalImpl{tty, fd, existingTermios, newTermios});
}

Terminal::~Terminal() {
    tcsetattr(impl->fd, TCSAFLUSH, &impl->oldTermios);
    fclose(impl->tty);
}

void Terminal::putString(const std::string& str) {
    fprintf(this->impl->tty, "%s", str.c_str());
}

void Terminal::putHighlighted(const std::string& str) {
    fprintf(this->impl->tty, "\033[7m%s\033[0m", str.c_str());
}

void Terminal::putChar(char ch) {
    fputc(ch, this->impl->tty);
}

char Terminal::getChar() {
    char c;
    int i = read(this->impl->fd, &c, 1);
    if (i == 0) {
        return -1;
    } else if (i < 0) {
        throw std::runtime_error("read error");
    }
    return c;
}

int Terminal::getKeyCode() {
    return static_cast<int>(this->getChar());
}

void Terminal::moveCursor(CursorDirection direction, int n) {
    std::string formatString = "";
    switch (direction) {
        case CursorDirection::Up:
        formatString = "\033[%dA";
        break;
        case CursorDirection::Down:
        formatString = "\033[%dB";
        break;
        case CursorDirection::Forward:
        formatString = "\033[%dC";
        break;
        case CursorDirection::Back:
        formatString = "\033[%dD";
        break;

    }
    fprintf(this->impl->tty, formatString.c_str(), n);
}

void Terminal::moveToColumn(int n) {
    fprintf(this->impl->tty, "\033[%dG", n);
}

void Terminal::eraseDisplay() {
    fprintf(this->impl->tty, "\033[%dJ", 0);

}

void Terminal::eraseLine() {
    fprintf(this->impl->tty, "\033[%dK", 0);
}
