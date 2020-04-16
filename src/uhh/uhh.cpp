#include "uhh.h"

#include "uhh-config.h"
#include "uhh-git.h"
#include <memory>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

typedef std::unique_ptr<std::vector<std::string>> SearchResults;

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

    std::ifstream in(configPath);
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
    dropRepoPath(repoPath);

    Config cfg{configPath};
    cfg.load();
    int error = Git::clone(dir, cfg.get("repo"));

    if (error) {
        dropRepoPath(repoPath);
        printf("git clone error %d\n", error);
        exit(error);
    }
}

SearchResults Uhh::getSearchResults(
        std::ifstream* file,
        std::vector<std::string>& tokens,
        bool filterOut = false) {

    std::string cmd, note;
    std::unique_ptr<std::vector<std::string>> foundLines(
            new std::vector<std::string>());

    while (std::getline(*file, cmd) && std::getline(*file, note)) {
        bool found = true;
        for (int i = 0; found && i < tokens.size(); ++i) {
            found = !(cmd.find(tokens[i]) == std::string::npos &&
                    note.find(tokens[i]) == std::string::npos);
        }

        if ((filterOut && found) || (!filterOut && !found)) {
            continue;
        }

        foundLines->push_back(cmd);
        foundLines->push_back(note);
    }

    return foundLines;
}

std::unique_ptr<std::ifstream> Uhh::getTagFile(std::string& tagName) {
    fs::path tagFile = repoPath / tagName;

    if (fs::is_regular_file(tagFile)) {
        std::unique_ptr<std::ifstream> ptr(new std::ifstream(tagFile));
        return std::move(ptr);
    }

    return nullptr;
}

void Uhh::deleteFn(const std::vector<std::string>& args) {
    int ptr = 0;
    std::string tag = args[ptr++];
    std::unique_ptr<std::ifstream> file = getTagFile(tag);

    if (file == nullptr) {
        printf("No Entries found for %s\n", tag.c_str());
        return;
    }

    std::vector<std::string> tokens(args.begin() + 1, args.end());

    // TODO: Stop being so bad at this. And just return the results you want.
    // TODO: Please stop being such this.

    std::ifstream* f = file.get();
    SearchResults res = getSearchResults(f, tokens, true);
    std::cout << "About to start over???\n";
    file->seekg(0, std::ios::beg); // THIS DID NOT WORKC>NCON>EHUNTOEHJUNTO
    std::cout << "About to start over???\n";
    SearchResults deleted = getSearchResults(f, tokens);
    std::cout << "About to start over???\n";
    file->close();

    std::cout << "Are you sure you want to delete the following items?\n";
    for (auto it : *deleted) {
        std::cout << it;
    }

    std::cout << "y for yes, everything else will throw an error.";
    std::string in;
    std::cin >> in;

    if (in != "y") {
        throw std::runtime_error("You couldn't help yourself could you?");
    }

    std::ofstream out(repoPath / tag);
    for (auto it : *res) {
        out << it;
    }

    out.flush();
    out.close();
}

void Uhh::find(const std::vector<std::string>& args) {

    int ptr = 0;
    std::string tag = args[ptr++];
    std::unique_ptr<std::ifstream> file = getTagFile(tag);

    if (file == nullptr) {
        printf("No Entries found for %s\n", tag.c_str());
        return;
    }

    std::vector<std::string> tokens(args.begin() + 1, args.end());

    SearchResults res = getSearchResults(file.get(), tokens);

    for (int i = 0; i < res->size(); i += 2) {
        if (i > 0) {
            printf("\n");
        }

        std::string cmd = res->at(i);
        if (cmd.back() == '\n') {
            cmd = cmd.substr(0, cmd.size() - 1);
        }

        printf("%s", cmd.c_str());
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

void Uhh::findConfigDirectory() {
    char* configDir;
    if ((configDir = getenv("UHH_DIR")) != nullptr) {
        if (fs::is_directory(configDir))
            dir = std::string(configDir);
        else {
            std::cout << configDir << " is not a valid directory." << std::endl
                << "Make sure to set the UHH_DIR environment variable to a valid value in " << std::endl
                << "your shell startup file (~/.bashrc, ~/.zshrc, etc) or in your ~/.profile, e.g." << std::endl
                << "export UHH_DIR=\u001b[31m<your desired path>\u001b[0m" << std::endl;
            exit(1);
        }

    }
    else if ((configDir = getenv("XDG_CONFIG_HOME")) != nullptr) {
        if (fs::is_directory(configDir))
            dir = std::string(configDir) + "/uhh";
        else {
            std::cout << "Your \u001b[31mXDG_CONFIG_HOME\u001b[0m variable is currently \u001b[31m" <<
                configDir << "\u001b[0m, but it is not a valid directory." << std::endl
                << "Please make the directory and try again." << std::endl;
            exit(1);
        }
    }
    else {
        dir = std::string(options.basePath) + "/.config/uhh";
    }

    configPath = dir / ".config";
    repoPath = dir / "repo";
}

Uhh::Uhh(UhhOpts &opts) {
    this->options = opts;

    findConfigDirectory();

    initialized = fs::is_directory(repoPath);

    readyDirectory();
    initPreferences();
    readPreferences();

    git_libgit2_init();
    if (!initialized) {
        readyGit();
    }
}

