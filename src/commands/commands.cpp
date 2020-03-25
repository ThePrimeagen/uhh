#include "commands.h"

Commands::Commands() {
    cmdList = {};
}

// TODO: destory command list on close
Commands::~Commands() {
}

void Commands::add(const CommandInfo* command) {
    cmdList[command->name] = command;
}

void Commands::add(const std::string name, const std::string usage, const commandFunction* func) {
    const CommandInfo info = {
            .name = name,
            .usage = usage,
            .command = func
    };

    cmdList[name] = &info;
}


const CommandInfo* Commands::get(const std::string name) {
    if(cmdList.find(name) == cmdList.end()) {
        return nullptr;
    }

    return cmdList.find(name)->second;
}

int Commands::get(const std::string name, std::vector<std::string> argv) {
    auto cmd = get(name);
    if(cmd == nullptr) {
        return 0;
    }

    return cmd->command(argv);
}

const CommandInfo* Commands::operator[](const std::string name) {
    return get(name);
}

void Commands::operator +(const CommandInfo* command) {
    add(command);
}
