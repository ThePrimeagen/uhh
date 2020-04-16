#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
#include "uhh.h"
#include "commands.h"
#include "uhh-config.h"

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

int syncFunction(Uhh& uhh, const std::string name, const std::vector<std::string> args) {
    uhh.sync();
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

// TODO: key words for find and add
int deleteFunction(Uhh& uhh, const std::string name, const std::vector<std::string> args) {
    if (args.size() == 0) {
        printf("invalid usage ./uhh delete {search tokens} \n");
        return 1;
    }
    uhh.deleteFn(args);
    return 0;
}

int findFunction(Uhh& uhh, const std::string name, const std::vector<std::string> args) {
    if (args.size() == 0) {
        printf("invalid usage ./uhh {tag} {search tokens} \n");
        return 1;
    }

    uhh.find(args);

    return 0;
}

#define buildCommand(name, usage, function) {name, usage, function}

int main(int argc, char **argv) {
    std::vector<std::string> commandArgs(argv + 1, argv + argc);
    Commands handler;

    const CommandInfo help = buildCommand("help", "help", helpFunction);
    handler + help;

    const CommandInfo about = buildCommand("about", "about", aboutFunction);
    handler + about;

    const CommandInfo add = buildCommand("add", "add", addFunction);
    handler + add;

    const CommandInfo find = buildCommand("find", "find", findFunction);
    handler + find;

    const CommandInfo sync = buildCommand("sync", "sync", syncFunction);
    handler + sync;

    // GOING PEE PEE, Dont worry i wont wash the hands...
    // oeuntonteuhntoehunotehu
    const CommandInfo deleteFn = buildCommand("delete", "delete", deleteFunction);
    handler + deleteFn;

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

    if (handler.has(cmd)) {
        commandArgs.erase(commandArgs.begin());
        return handler.call(uhh, cmd, commandArgs);
    }

    return handler.call(uhh, "find", commandArgs);
}
