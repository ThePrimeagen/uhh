#pragma once

#include <memory>
#include <iostream>
#include <vector>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

struct UhhOpts {
    std::string basePath;
    bool verbose = false;
};

struct UhhConfig {
    std::string repoUrl;
    bool syncOnAdd;
};

using std::unique_ptr;
using std::vector;
using std::string;
using std::ifstream;

class Uhh {
    public:
        Uhh(UhhOpts& opts);

        void addCommand(const string& tag, const string& cmd, const string& note);
        void find(const vector<string>& args);
        void sync();
        void deleteFn(const vector<string>& args);

    private:
        bool initialized;

        UhhOpts options;
        UhhConfig config;

        fs::path dir;
        fs::path configPath;
        fs::path repoPath;

        void readyDirectory();
        void initPreferences();
        void readPreferences();
        void readyGit();
        void findConfigDirectory();

        unique_ptr<vector<string>> getSearchResults(ifstream*, vector<string>&, bool);
        unique_ptr<ifstream> getTagFile(string&);
};
