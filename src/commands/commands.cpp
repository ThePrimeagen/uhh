#include "commands.h"

Commands::Commands() {
    cmdList = {};
}

// TODO: destory command list on close
Commands::~Commands() {
}

void Commands::add(const std::string name, commandFunction* func) {
    cmdList[name] = func;
}

commandFunction* Commands::get(const std::string name) {
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

    return cmd(argv);
}

commandFunction* Commands::operator[](const std::string name) {
    return get(name);
}

void Commands::operator +(std::tuple<std::string, commandFunction*> newFunc) {
    add(std::get<0>(newFunc), std::get<1>(newFunc));
}
