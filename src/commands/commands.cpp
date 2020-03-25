#include "commands.h"

Commands::Commands() {
    cmdList = {};
}

void Commands::add(const CommandInfo command) {
    // cmdList[command.name] = command;
    cmdList.insert({
        command.name,
        command
    });
}

void Commands::add(const std::string name, const std::string usage, std::function<commandFunction> func) {
    // cmdList[command.name] = command;
    cmdList.insert({
        name,
        CommandInfo {
            .name = name,
            .usage = usage,
            .func = func
        }
    });
}


const CommandInfo Commands::get(const std::string name) {
    if(cmdList.find(name) == cmdList.end()) {
        return CommandInfo {};
    }

    return cmdList.find(name)->second;
}

const int Commands::call(const std::string name, const std::vector<std::string> args) {
    if(cmdList.find(name) == cmdList.end()) {
        return 1;
    }

    return cmdList.find(name)->second.func(args);
}


const CommandInfo Commands::operator[](const std::string name) {
    return get(name);
}

void Commands::operator +(const CommandInfo command) {
    add(command);
}
