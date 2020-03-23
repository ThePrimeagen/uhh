#include "setup.h"

#include <string>
#include <filesystem>
#include <iostream>
#include "git2.h"

namespace fs = std::filesystem;

void readyDirectory(Uhh* uhh) {
    if (fs::is_directory(uhh->dir)) {
        return;
    }

    fs::create_directory(uhh->dir);
}

bool initPreferences(Uhh* uhh) {
    if (fs::is_regular_file(uhh->configPath)) {
        return false;
    }

    std::ofstream ofs(uhh->configPath);

    std::string gitRepo;
    std::cout << "Create config, please enter your git repo:";
    std::cin >> gitRepo;

    // git@github.com:ThePrimeagen/one-liners.git
    ofs << "git " << gitRepo << "\n";
    ofs.close();

    return true;
}

void readyGit(Uhh* uhh, bool shouldInitGit) {
    if (!shouldInitGit) {
        return;
    }

}

Uhh::Uhh(std::string& base) {
    dir = base + "/.uhh";
    configPath = dir + "/.config";
    repoPath = dir + "/repo";

    readyDirectory(this);
    readyGit(this, initPreferences(this));
}

