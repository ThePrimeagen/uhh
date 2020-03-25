#pragma once

#include <map> 
#include <string>

typedef int commandFunction(const char**);

class commands {
    private:
        std::map<std::string, commandFunction*> cmdList;

    public:
        commands();
        ~commands();
        void add(const std::string name, commandFunction* func);
        commandFunction* get(const std::string name);
        // call function
        int get(const std::string name, const char**);

    // Operator overloading
    public:
        commandFunction* operator [](std::string name);
        void operator +(std::tuple<std::string, commandFunction*> newFunc);
};