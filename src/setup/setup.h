#pragma once

#include <string>

struct UhhOpts {
    std::string basePath;
    bool verbose = false;
};

struct UhhConfig {
    std::string repoUrl;
};

class Uhh {
    public:
        Uhh(UhhOpts& opts);

        void addCommand(const std::string& tag, const std::string& cmd, const std::string& note);
        void find(const std::string& tag, const std::string& needle = "");

    private:
        bool initialized;
        UhhOpts options;

        std::string dir;
        std::string configPath;
        std::string repoPath;
        UhhConfig config;

        void readyDirectory();
        void initPreferences();
        void readPreferences();
        void readyGit();
};
