#pragma once
#include <list>
#include "Poco/DOM/Node.h"

using Poco::XML::Node;

using namespace std;

class Segment
{
public:
    Segment(Poco::XML::Node*);
    ~Segment(void);

    string toString();

    string hash;
    long start;
    long end;
};

class Filehash
{
public:
    Filehash(const char*);
    ~Filehash(void);  

    string toString();

    list<Segment> *seg;
    string hash;
    long size;
};

