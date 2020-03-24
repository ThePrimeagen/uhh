#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
#include "setup/setup.h"

std::vector<std::string> helps {
    "uhh",
    "help",
    "huh",
    "wat",
    "what",
    "-h",
    "wut",
};

void printHelp() {
    printf("Look, its a help menu\n");
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printHelp();
        return 0;
    }

    std::string cmd(argv[1]);
    if (std::find(helps.begin(), helps.end(), cmd.c_str()) != helps.end()) {
        printHelp();
        return 0;
    }

    std::string home = std::string(getenv("HOME"));
    UhhOpts opts{
        home,
        cmd
    };

    Uhh uhh{opts};

    if (cmd.compare("add") == 0) {
        std::string tag, cmd, note;
        std::cout << "What is the tag? ";
        std::getline(std::cin, tag);
        std::cout << "What is the command? ";
        std::getline(std::cin, cmd);
        std::cout << "What is the note? ";
        std::getline(std::cin, note);

        uhh.addCommand(tag, cmd, note);
    } else {
        std::stringstream str;
        int i = argc - 2;
        int idx = 1;

        for (int i = 2; i < argc; ++i) {
            str << argv[i];
            if (i + 1 < argc) {
                str << " ";
            }
        }

        uhh.find(cmd, str.str());
    }

    return 0;
}
