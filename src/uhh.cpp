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


int helpFunction(std::vector<std::string> args) {
    printf("Look, its a help menu\n");
    return 0;
}

int main(int argc, char** argv) {
    std::vector<std::string> commandArgs(argv + 1, argv + argc);

    Commands handler;
    handler + std::make_tuple("help", &helpFunction);

    if (commandArgs.size() == 0) {
        return handler["help"](commandArgs);
    }

    std::string cmd(commandArgs[0]);
    if (std::find(helps.begin(), helps.end(), cmd.c_str()) != helps.end()) {
        return handler["help"](commandArgs);
    }

    std::string home = std::string(getenv("HOME"));
    UhhOpts opts{
        home,
        cmd
    };

    Uhh uhh{opts};

    // remove are command
    commandArgs.erase(commandArgs.begin());

    if(handler.get(cmd, commandArgs) == 0) {
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
