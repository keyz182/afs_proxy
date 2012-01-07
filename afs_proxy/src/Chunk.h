#pragma once
#include "Poco/Runnable.h"
#include "Poco/TemporaryFile.h"
#include <vector>

class Chunk :
    public Poco::Runnable
{
public:
    Chunk(std::vector<std::string> endpoints, std::string hash, long start, long end);
    ~Chunk(void);

    void run();

    void getData(std::string, long start, long end);

    bool failed;
    
    Poco::TemporaryFile tmp;
    std::vector<std::string> endpoints;
    std::string hash;
    std::string dataHash;
    long start;
    long end;
};

