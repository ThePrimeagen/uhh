#pragma once

#include "uhh.h"
#include "git2.h"
#include "git2/credential.h"
#include "git2/remote.h"

namespace fs = std::filesystem;

class Git {
    public:
        static int clone(const fs::path& configPath, const std::string& repoUrl);
        static int commit(const UhhConfig& config, const std::string& repoPath);
        static void printError(int error);
};

