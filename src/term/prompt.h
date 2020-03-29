#pragma once

#include "term/terminal.h"

#include <string>

class Prompt {
  public:
    Prompt(Terminal& term, const std::string& prompt);

    bool handleKey(const Terminal::Key key);
    const int currentColumn() { return prompt.length() + input.length() + 1; }
    const std::string query() { return input; }

private:
    Terminal& term;
    std::string prompt;
    std::string input;
};
