#include <string>

namespace my_strings {

std::string operator*(std::string const &in, size_t m) {
    std::string res;
    res.reserve(in.size() * m + 1);

    for (size_t i = 0; i < m; i++)
        res += in;
    return res;
}

std::string operator*(size_t m, std::string const &in) {
    std::string res;
    res.reserve(in.size() * m + 1);

    for (size_t i = 0; i < m; i++)
        res += in;
    return res;
}
}