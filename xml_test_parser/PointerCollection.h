#pragma once
#include <list>
#include "Poco/DOM/Node.h"

using Poco::XML::Node;

using namespace std;

class DataDescription
{
public:
    DataDescription(Poco::XML::Node*);
    ~DataDescription(void);

    std::string id;
    std::string name;
    std::string project;
    std::string description;

    std::string toString();
};

class Endpoint
{
public:
    Endpoint(Poco::XML::Node*);
    ~Endpoint(void);

    std::string url;
    std::string meta;
    bool accessible;


    std::string toString();
};

class DataPointer
{
public:
    DataDescription *dd;
    list<Endpoint> *ep;
    DataPointer(Poco::XML::Node*);
    ~DataPointer(void);

    std::string toString();
};

class PointerCollection
{
public:
    list<DataPointer> *dp;

    PointerCollection(const char*);
    ~PointerCollection(void);

    std::string toString();
};


