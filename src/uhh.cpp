#include <vector>
#include <algorithm>
#include "setup/setup.h"


std::vector<std::string> helps {
    "uhh",
    "help",
    "huh",
    "wat",
    "what",
    "wut",
};

void printHelp() {
    printf("Look, its a help menu\n");
}

int main(int argc, char** argv) {
    printf("Commands %d\n", argc);

    if (argc < 2) {
        printHelp();
        return 0;
    }

    std::string cmd(argv[1]);
    if (std::find(helps.begin(), helps.end(), cmd.c_str()) != helps.end()) {
        printHelp();
        return 0;
    }

    std::string home = std::string(getenv("HOME"));
    UhhOpts opts{
        home,
        cmd
    };

    Uhh* uhh = new Uhh(opts);

    return 0;
}
