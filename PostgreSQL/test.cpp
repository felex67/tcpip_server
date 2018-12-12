#include <iostream>
#include <stdio.h>
#include <time.h>
#include "querries.hpp"

std::string& operator+(std::string s, const double d) {
    char buff[20];
    std::string t;
    sprintf(buff, "%0.4f", d);
    return (s += buff);
}
std::string& operator+(std::string &s, const int i) {
    char buff[20];
    sprintf(buff, "%.d", i);
    return s += buff;
}
std::string& operator+=(std::string &s, const double &d) {
    s = (s + d);
    return s;
}
std::string& operator+=(std::string &s, const int &d) {
    s = (s + d);
    return s;
}

int main() {
    Order o;
    std::cout << (4000 > 3999.999999 ? "true" : "false") << "\n";
    return 0;
}