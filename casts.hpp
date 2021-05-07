#pragma once

#include <sstream>
#include <string_view>

int string2int(const std::string_view& string) {
    std::stringstream ss;
    ss << string;
    int result;
    ss >> result;
    return result;
}

long double string2longdouble(const std::string_view& string) {
    std::stringstream ss;
    ss << string;
    long double result;
    ss >> result;
    return result;
}

std::string int2string(int x) {
    std::stringstream ss;
    ss << x;
    std::string s;
    ss >> s;
    return s;
}