#include "uhh.h"

#include "uhh-config.h"
#include "uhh-git.h"
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

void Uhh::readyDirectory() {
    if (fs::is_directory(dir)) {
        return;
    }

    fs::create_directory(dir);
}

void Uhh::initPreferences() {
    if (fs::is_regular_file(configPath)) {
        return;
    }

    std::string gitRepo;
    std::cout << "Create config, please enter your git repo:";
    std::cin >> gitRepo;

   Config cfg{configPath};
   cfg.set("repo", gitRepo);

   cfg.save();
}

void Uhh::readPreferences() {
    Config cfg {configPath};
    if(!cfg.load()) {
        throw std::runtime_error("Could not open file");
    }

    config.repoUrl = cfg["repo"];
    config.syncOnAdd = cfg["sync-on-add"] == "true";
}

void dropRepoPath(std::string repoPath) {
    if (fs::is_directory(repoPath)) {
        fs::remove_all(repoPath);
    }
}

void Uhh::readyGit() {
    if (initialized) {
        return;
    }

    dropRepoPath(repoPath);

    int error = Git::clone(config, repoPath);

    if (error) {
        dropRepoPath(repoPath);
        Git::printError(error);
        exit(error);
    }
}

std::string Uhh::get(const std::string tag) {
    fs::path repo(repoPath);
    fs::path tagFile = repo / tag;

    if (fs::is_regular_file(tagFile)) {
        return tagFile;
    }

    printf("No Entries found for %s\n", tag.c_str());
    return "";
}

void Uhh::find(const std::vector<std::string>& args) {
    int ptr = 0;
    std::string tag = args[ptr++];

    fs::path repo(repoPath);
    fs::path tagFile = repo / tag;

    std::ifstream in;

    if (fs::is_regular_file(tagFile)) {
        in.open(tagFile, std::ios::app);
    }
    else {
        printf("No Entries found for %s\n", tag.c_str());
        return;
    }


    std::vector<std::string> needle(args.begin() + 1, args.end());
    std::string cmd, note;

    while (std::getline(in, cmd) && std::getline(in, note)) {
        if (!needle.empty()) {
            bool found = true;
            for (int i = ptr; found && i < needle.size(); ++i) {
                found = !(cmd.find(needle[i]) == std::string::npos &&
                    note.find(needle[i]) == std::string::npos);
            }

            if (!found) {
                continue;
            }
        }

        if (options.verbose) {
            printf("Note: %s\n", note.c_str());
            printf("Cmd: %s\n", cmd.c_str());
        } else {
            printf("%s\n", cmd.c_str());
        }
    }
}

void Uhh::sync() {
    int error = Git::commit(config, repoPath);

    if (error) {
        printf("You dun messed up with your system commands.  You should probably fix this. (by doing it right).\n");
        printf("This was during the git commit process, so go look at it: Error code: %d and strerror: %s\n", error, strerror(errno));
        exit(error);
    }
}

void Uhh::addCommand(const std::string& tag, const std::string& cmd, const std::string& note) {
    // TODO: Reserverd words.  You cannot have commands with help, list, update, sync

    // I GET IT IT COULD BE BETTER
    fs::path repo(repoPath);
    fs::path tagFile = repo / tag;

    Config cfg{ tagFile };
    if (fs::is_regular_file(tagFile)) {
        if(!cfg.load()) {
            throw std::runtime_error("Could not open file");
        }
    }

    if (cmd.back() != '\n') {
        cfg.set("command", cmd);
    }

    if (note.back() != '\n') {
        cfg.set("note", note);
    }

    cfg.save();

    if (config.syncOnAdd) {
        sync();
    }
}

Uhh::Uhh(UhhOpts &opts) {
    options = opts;
    dir = opts.basePath + "/.uhh";
    configPath = dir + "/.config";
    repoPath = dir + "/repo";

    initialized = fs::is_directory(repoPath);

    readyDirectory();
    initPreferences();
    readPreferences();

    git_libgit2_init();
    if (!initialized) {
        readyGit();
    }
}

