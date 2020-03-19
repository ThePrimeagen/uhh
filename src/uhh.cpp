#include <iostream>
#include <fstream>
#include <dirent.h>
#include <cstdlib>
#include <string>

using namespace std;

bool isDirectory(const char* pzPath) {
    if (pzPath == NULL) {
        return false;
    }

    DIR *pDir;
    bool bExists = false;

    pDir = opendir(pzPath);

    if (pDir != NULL) {
        bExists = true;
        closedir(pDir);
    }

    return bExists;
}

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    string HOME = getenv("HOME");

    if (HOME.empty()) {
        printf("Home is where the heart is, ya dingus\n");
        return 1;
    }

    string uhhDir = HOME + "/.uhh";

    if (!isDirectory(uhhDir.c_str())) {
        printf("~/.uhh does not exist.  Creating empty .uhh directory\n");
        system(("mkdir " + uhhDir).c_str());
    }

    // TODO: OHHH BAD NAMES
    string uhhConfig = uhhDir + "/.config";
    ifstream uhhC;
    uhhC.open(uhhConfig);

    if (!uhhC.good()) {
        printf("~/.uhh does not have a config.  Will not be able to save to git\n");
        system(("touch " + uhhDir + ".config").c_str());
    }

    // TODO: If there is no repo, then save flat file.
    // TODO: If there is a repo added that does not exist, clone it
    // TODO: If there is a flat file when cloning, pull, attempt to merge, ya ya

    return 0;
}
