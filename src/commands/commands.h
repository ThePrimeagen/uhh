#pragma once

#include <map> 
#include <string>
#include <vector>

typedef int commandFunction(const std::vector<std::string>);

struct CommandInfo {
    const std::string name; 
    const std::string usage;
    const commandFunction* command;
};


class Commands {
    private:
        std::map<std::string, const CommandInfo*> cmdList;

    public:
        Commands();
        ~Commands();
        void add(const CommandInfo* command);
        void add(const std::string name, const std::string usage, const commandFunction* func);

        const CommandInfo* get(const std::string name);
        // call function
        int get(const std::string name, const std::vector<std::string>);

    // Operator overloading
    public:
        const CommandInfo* operator [](const std::string name);
        void operator +(const CommandInfo* command);
};