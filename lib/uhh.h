#pragma once

#include <vector>
#include <string>

struct UhhOpts {
    std::string basePath;
    bool verbose = false;
};

struct UhhConfig {
    std::string repoUrl;
    bool syncOnAdd;
};

class Uhh {
    public:
        Uhh(UhhOpts& opts);

        void addCommand(const std::string& tag, const std::string& cmd, const std::string& note);
        void find(const std::vector<std::string>& args);
        void sync();

    private:
        bool initialized;

        UhhOpts options;
        UhhConfig config;

        std::string dir;
        std::string configPath;
        std::string repoPath;

        void readyDirectory();
        void initPreferences();
        void readPreferences();
        void readyGit();
        void findConfigDirectory();
};
