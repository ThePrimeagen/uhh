#include "uhh.h"

#include <string>
#include <filesystem>
#include <vector>
#include <fstream>
#include <iostream>
#include "uhh-git.h"
#include <cstring>

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

    std::ofstream ofs(configPath);

    std::string gitRepo;
    std::cout << "Create config, please enter your git repo:";
    std::cin >> gitRepo;

    // git@github.com:ThePrimeagen/one-liners.git
    ofs << "git " << gitRepo << "\n";
    ofs.close();
}

void Uhh::readPreferences() {
    std::ifstream infile(configPath);

    std::string line;
    config.syncOnAdd = false;

    while (std::getline(infile, line)) {
        std::istringstream tokenizer(line);

        std::string item, value;
        tokenizer >> item >> value;

        // probably should make this much nicer.......
        // I hate these things.
        if (item.compare("git") == 0) {
            config.repoUrl = value;
        }
        else if (item.compare("sync-on-add")) {
            config.syncOnAdd = value == "true";
        }
    }
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

    std::ofstream out;

    if (fs::is_regular_file(tagFile)) {
        out.open(tagFile, std::ios::app);
    } else {
        std::ofstream ofs(tagFile);
        out.open(tagFile, std::ios::app);
    }

    if (!out.good()) {
        throw std::runtime_error("Could not open file");
    }

    out << cmd;
    if (cmd.back() != '\n') {
        out << '\n';
    }

    out << note;
    if (note.back() != '\n') {
        out << '\n';
    }
    out.close();

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

