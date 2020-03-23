#pragma once

#include <fstream>
#include <string>

struct UhhOpts {
    std::string basePath;
    std::string command;
};

struct UhhConfig {
    std::string repoUrl;
};

class Uhh {
    public:
        Uhh(UhhOpts& opts);
        ~Uhh();

    private:
        bool initialized;

        std::string dir;
        std::string configPath;
        std::string repoPath;
        UhhConfig config;

        void readyDirectory();
        void initPreferences();
        void readPreferences();
        void readyGit();
};
