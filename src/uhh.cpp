#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
#include "setup/setup.h"
#include "commands/commands.h"

std::vector<std::string> helps {
    "uhh",
    "help",
    "huh",
    "wat",
    "what",
    "-h",
    "wut",
};


int helpFunction(char **argv) {
    printf("Look, its a help menu\n");
    return 1;
}

int main(int argc, char** argv) {
    cd .
    if (argc < 2) {
        return handler["help"](argv);
    }

    std::string cmd(argv[1]);
    if (std::find(helps.begin(), helps.end(), cmd.c_str()) != helps.end()) {
        return handler["help"](argv);
    }

    std::string home = std::string(getenv("HOME"));
    UhhOpts opts{
        home,
        cmd
    };

    Uhh uhh{opts};

    if(handler.get(cmd, argv) == 0) {
        return 1;
    }
    
    /*
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
    */

    
    return 0;
}
