#pragma once

#include <string>

class HTTPUtils
{
public:
    HTTPUtils(void);
    ~HTTPUtils(void);

    static std::string get(std::string, long start = -1, long end = -1);
};

