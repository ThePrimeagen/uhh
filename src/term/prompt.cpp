#include "term/prompt.h"

#include <iostream>
#include <iterator>


Prompt::Prompt(Terminal& term, const std::string& prompt)
    : term{term}, prompt{prompt}, input{} {
    term.putString((prompt + "\n").c_str());
}

bool Prompt::handleKey(const Terminal::Key key) {
    bool resetSelection = true;

    if (isprint(key)) {
        input.push_back(key);
    } else if (input.length() > 0) {
        if (key == Terminal::DEL || key == Terminal::BACKSPACE) {
            input.pop_back();
        } else if (key == Terminal::CTRL_U) {
            input.clear();
        }
        resetSelection = false;
    } else {
        resetSelection = false;
    }
    term.moveToColumn(1);
    term.eraseLine();
    term.putString(prompt + input);
    return resetSelection;
}
