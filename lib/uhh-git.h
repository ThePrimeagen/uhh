#include "uhh.h"
#include "git2.h"
#include "git2/credential.h"
#include "git2/remote.h"

class Git {
    public:
        static int clone(const UhhConfig& config, const std::string& repoPath);
        static int commit(const UhhConfig& config, const std::string& repoPath);
        static void printError(int error);
};

