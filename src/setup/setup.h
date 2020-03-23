#pragma once

#include <fstream>
#include <string>

class Uhh {
    public:
        std::string dir;
        std::string configPath;
        std::string repoPath;

        Uhh(std::string& base);
        ~Uhh();

    private:
        bool uninitialized;
};

