#include "commands.h"
#include "iostream"

Commands::Commands()
{
    cmdList = {};
}

void Commands::add(const CommandInfo command)
{
    // cmdList[command.name] = command;
    cmdList.insert({command.name,
                    command});
}

void Commands::add(const std::string name, const std::string usage, std::function<commandFunction> func)
{
    // cmdList[command.name] = command;
    cmdList.insert({name,
                    CommandInfo{
                        .name = name,
                        .usage = usage,
                        .func = func}});
}

const std::optional<CommandInfo> Commands::get(const std::string name)
{
    auto found = cmdList.find(name);
    if (found == cmdList.end())
    {
        return {};
    }

    return found->second;
}

const int Commands::call(const std::string name, const std::vector<std::string> args)
{
    auto found = get(name).value_or(CommandInfo{
        .name = "Not Found",
        .usage = "Unknown",
        .func = notFoundFunction});
    return found.func(name, args);
}

const CommandInfo Commands::operator[](const std::string name)
{
    return get(name).value_or(CommandInfo{
        .name = "Not Found",
        .usage = "Unknown",
        .func = notFoundFunction});
}

void Commands::operator+(const CommandInfo command)
{
    add(command);
}

int Commands::notFoundFunction(const std::string command, const std::vector<std::string> args)
{
    printf("%s not found", command.c_str());
    return 1;
}