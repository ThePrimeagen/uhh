#pragma once

#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <string>

class Config {
    private:
        std::map<std::string, std::string> values;
        std::string file;
    public:
        Config(std::string file) {
            this->file = file;
            this->values = {};
        }

        void set(std::string key, std::string value);
        std::string get(std::string key);

        // parses the config file
        bool load();
        // saves the config file
        void save();

    //operators
    public:
        std::string operator [](std::string key);
};
