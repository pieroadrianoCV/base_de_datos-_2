#include "helpers.h"

namespace helpers {
    std::string capitalize(std::string str) {
        if (!str.empty()) {
            str[0] = toupper(str[0]);
            transform(str.begin() + 1, str.end(), str.begin() + 1, [](unsigned char c) { return tolower(c); });
        }
        return str;
    }
}
