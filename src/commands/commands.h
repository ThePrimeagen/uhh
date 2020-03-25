#pragma once

#include <map> 
#include <string>

typedef int commandFunction(char**);

class Commands {
    private:
        std::map<std::string, commandFunction*> cmdList;

    public:
        Commands();
        ~Commands();
        void add(const std::string name, commandFunction* func);
        commandFunction* get(const std::string name);
        // call function
        int get(const std::string name, char**);

    // Operator overloading
    public:
        commandFunction* operator [](std::string name);
        void operator +(std::tuple<std::string, commandFunction*> newFunc);
};