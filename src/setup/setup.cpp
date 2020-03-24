#include "setup.h"

#include <string>
#include <filesystem>
#include <iostream>
#include "git2.h"
#include "git2/credential.h"
#include "git2/remote.h"

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
    while (std::getline(infile, line)) {
        std::istringstream tokenizer(line);

        std::string item, value;
        tokenizer >> item >> value;

        if (item.compare("git") == 0) {
            config.repoUrl = value;
        }
    }
}

void dropRepoPath(std::string repoPath) {
    if (fs::is_directory(repoPath)) {
        fs::remove_all(repoPath);
    }
}

int credentials_cb(git_cred **out, const char *url, const char *username_from_url,
                   unsigned int allowed_types, void *payload) {
    // allowed_types == bitmask
    // TODO: Make this work so its like extensibal brah
    // GIT_CREDTYPE_*

    return git_credential_ssh_key_from_agent(out, username_from_url);
}

void Uhh::readyGit() {
    if (initialized) {
        return;
    }

    dropRepoPath(repoPath);

    git_repository *repo = NULL;
    const char *url = config.repoUrl.c_str();
    const char *path = repoPath.c_str();

    git_clone_options clone_opts = GIT_STATUS_OPTIONS_INIT;
    git_fetch_options fetch_opts = GIT_FETCH_OPTIONS_INIT;
    fetch_opts.callbacks.credentials = credentials_cb;

    clone_opts.fetch_opts = fetch_opts;

    int error = git_clone(&repo, url, path, &clone_opts);

    if (error) {
        dropRepoPath(repoPath);
        const git_error *e = git_error_last();
        printf("Error %d/%d: %s\n", error, e->klass, e->message);
        exit(error);
    }
}

void Uhh::find(const std::string& tag, const std::string& needle) {
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

    std::string cmd, note;
    while (std::getline(in, cmd) && std::getline(in, note)) {
        if (!needle.empty()) {
            if (cmd.find(needle) == std::string::npos &&
                note.find(needle) == std::string::npos) {
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

void Uhh::addCommand(const std::string& tag, const std::string& cmd, const std::string& note) {
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
}

Uhh::Uhh(UhhOpts &opts) {
    options = opts;
    dir = opts.basePath + "/.uhh";
    configPath = dir + "/.config";
    repoPath = dir + "/repo";

    initialized = fs::is_directory(repoPath);

    readyDirectory();
    initPreferences();

    if (!initialized) {
        git_libgit2_init();
        readPreferences();
        readyGit();
    }
}

