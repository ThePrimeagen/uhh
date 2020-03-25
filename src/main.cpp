#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
#include "uhh.h"
#include "commands.h"

std::vector<std::string> helps{
    "uhh",
    "help",
    "huh",
    "wat",
    "what",
    "-h",
    "wut",
};

int helpFunction(Uhh& uhh, const std::string name, const std::vector<std::string> args) {
    printf("Look, its a help menu\n");
    return 0;
}

int aboutFunction(Uhh& uhh, const std::string name, const std::vector<std::string> args) {
    printf("Uhh created by ThePrimeagen @ https://github.com/ThePrimeagen/uhh\n");
    return 0;
}

int addFunction(Uhh& uhh, const std::string name, const std::vector<std::string> args) {

    std::string tag, cmd, note;
    std::cout << "What is the tag? ";
    std::getline(std::cin, tag);
    std::cout << "What is the command? ";
    std::getline(std::cin, cmd);
    std::cout << "What is the note? ";
    std::getline(std::cin, note);

    uhh.addCommand(tag, cmd, note);

    return 0;
}

int findFunction(Uhh& uhh, const std::string name, const std::vector<std::string> args) {
    if (args.size() == 0) {
        printf("invalid usage ./uhh find {tag} {search} \1");
        return 1;
    }

    std::stringstream str;

    for (int i = 1; i < args.size(); ++i) {
        str << args[i];
    }

    uhh.find(args[0], str.str());

    return 0;
}

int main(int argc, char **argv) {
    std::vector<std::string> commandArgs(argv + 1, argv + argc);
    Commands handler;

    // TODO: make this a macro someday...
    const CommandInfo help = {
        .name = "help",
        .usage = "help",
        .func = helpFunction,
    };
    handler + help;

    const CommandInfo about = {
        .name = "about",
        .usage = "about",
        .func = aboutFunction,
    };
    handler + about;

    const CommandInfo add = {
        .name = "add",
        .usage = "add",
        .func = addFunction,
    };
    handler + add;

    const CommandInfo find = {
        .name = "find",
        .usage = "find",
        .func = findFunction,
    };
    handler + find;

    std::string home = std::string(getenv("HOME"));
    UhhOpts opts{
        home,
    };
    Uhh uhh(opts);

    if (commandArgs.size() == 0) {
        return handler.call(uhh, "help", commandArgs);
    }

    std::string cmd(commandArgs[0]);
    if (std::find(helps.begin(), helps.end(), cmd.c_str()) != helps.end()) {
        return handler.call(uhh, "help", commandArgs);
    }

    // remove are command
    commandArgs.erase(commandArgs.begin());

    return handler.call(uhh, cmd, commandArgs);
}
