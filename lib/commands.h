#pragma once

#include <map>
#include <string>
#include <vector>
#include <functional>
#include <optional>
#include "uhh.h"

typedef int commandFunction(Uhh& uhh, const std::string command, const std::vector<std::string>);

struct CommandInfo {
    const std::string name;
    const std::string usage;
    const std::function<commandFunction> func;
};


class Commands {
    private:
        std::map<std::string, const CommandInfo> cmdList;

    public:
        Commands();
        void add(const CommandInfo command);
        void add(const std::string name, const std::string usage, std::function<commandFunction> func);

        const std::optional<CommandInfo> get(const std::string name);
        const int call(Uhh& uhh, const std::string name, const std::vector<std::string> args);

    // Operator overloading
    public:
        const CommandInfo operator [](const std::string name);
        void operator +(const CommandInfo command);

    private:
        static int notFoundFunction(Uhh& uhh, const std::string command, const std::vector<std::string> args);
};
