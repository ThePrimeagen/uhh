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

Uhh::Uhh(UhhOpts &opts) {
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

