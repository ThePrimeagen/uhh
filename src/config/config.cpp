#include "uhh-config.h"

void Config::set(std::string name, std::string value) {
    values[name] = value;
}

std::string Config::get(std::string name) {
    auto a = values.find(name);
    if(a == values.end()) {
        return "";
    }

    return a->second;
}

bool Config::load() {
    std::ifstream cFile (this->file);
    if (cFile.is_open())
    {
        std::string line;
        while(getline(cFile, line)) {
            if(line[0] == '#' || line.empty())
                continue;
            auto delimiterPos = line.find("=");
            auto name = line.substr(0, delimiterPos);
            auto value = line.substr(delimiterPos + 1);

            values[name] = value;
        }

        return true;
    }

    return false;
}

void Config::save() {
    std::ofstream myfile (this->file);
    if (myfile.is_open())
    {
        auto it = values.begin();

        while (it != values.end())
        {
            std::string name = it->first;
            std::string value = it->second;

            myfile << name.c_str() << "=" << value.c_str() << "\n";

            it++;
        }
    }
}

std::string Config::operator[](std::string name) {
    return get(name);
}
