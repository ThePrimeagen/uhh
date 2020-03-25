#include "commands.h";


commands::commands() {
    cmdList = {};
}

// TODO: destory command list on close
commands::~commands() {
}

void commands::add(const std::string name, commandFunction* func) {
    cmdList[name] = func;
}

commandFunction* commands::get(const std::string name) {
    if(cmdList.find(name) == cmdList.end()) {
        return nullptr;
    }

    return cmdList.find(name)->second;
}

int commands::get(const std::string name, const char** argv) {
    auto cmd = get(name);
    if(cmd == nullptr) {
        return 0;
    }

    return cmd(argv);
}

commandFunction* commands::operator[](const std::string name) {
    return get(name);
}

void commands::operator +(std::tuple<std::string, commandFunction*> newFunc) {
    add(std::get<0>(newFunc), std::get<1>(newFunc));
}
