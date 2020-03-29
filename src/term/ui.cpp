#include "term/terminal.h"
#include "term/prompt.h"

#include <vector>

static const int MAX_RESULTS = 25;

int translateEscape(Terminal& term);

int main() {
    Terminal term;
    Prompt prompt{term, "> "};
    term.eraseDisplay();

    std::vector<std::string> items{"first", "second", "third", "fourth", "fifth"};
    int keyCode = -1;
    int selected = 1;
    for (;;) {
        int i=1;
        for (auto it = items.cbegin(); it != items.cend(); it++, i++) {
            if (i == selected) {
                term.putHighlighted(*it);
            } else {
                term.putString(*it);
            }
            if (i == MAX_RESULTS) {
                break;
            }

            term.putChar('\n');
        }
        term.moveCursor(CursorDirection::Up, i);
        term.moveToColumn(prompt.currentColumn());
        
        keyCode = term.getKeyCode();
        if (keyCode == Terminal::ESC) {
            keyCode = translateEscape(term);
        }

        if (keyCode == Terminal::CTRL_C || keyCode == Terminal::ENTER) {
            break;
        } else if (keyCode == Terminal::CTRL_N) {
            selected = std::min(selected + 1, i-1);
        } else if (keyCode == Terminal::CTRL_P) {
            selected = std::max(selected - 1, 0);
        } else if (prompt.handleKey(static_cast<Terminal::Key>(keyCode))) {
            selected = 1;
        }

        term.putChar('\n');
        term.eraseDisplay();
    }
    term.moveToColumn(1);
    term.eraseDisplay();

    if (keyCode == Terminal::ENTER) {
        fprintf(stdout, "%s\n", items[selected-1].c_str());
        return 0;
    }
    return 1;
}

int translateEscape(Terminal& term) {
    char ch = term.getChar();
    if (ch != '[') {
        return ch;
    }
    ch = term.getChar();
    if (ch == 'A' || ch == 'D') {
        return Terminal::CTRL_P;
    } else if (ch == 'B' || ch == 'C') {
        return Terminal::CTRL_N;
    }
    return 0;
}
