#pragma once

#include <map> 
#include <string>
#include <vector>
#include <functional>

typedef int commandFunction(const std::vector<std::string>);

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

        const CommandInfo get(const std::string name);
        const int call(const std::string name, const std::vector<std::string> args);

    // Operator overloading
    public:
        const CommandInfo operator [](const std::string name);
        void operator +(const CommandInfo command);
};