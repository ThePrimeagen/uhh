#include "setup/setup.h"

int main(void) {

    std::string home = std::string(getenv("HOME"));
    Uhh* uhh = new Uhh(home);

    return 0;
}
