#pragma once

#include <map> 
#include <string>
#include <vector>

typedef int commandFunction(const std::vector<std::string>);

class Commands {
    private:
        std::map<std::string, commandFunction*> cmdList;

    public:
        Commands();
        ~Commands();
        void add(const std::string name, commandFunction* func);
        commandFunction* get(const std::string name);
        // call function
        int get(const std::string name, const std::vector<std::string>);

    // Operator overloading
    public:
        commandFunction* operator [](std::string name);
        void operator +(std::tuple<std::string, commandFunction*> newFunc);
};