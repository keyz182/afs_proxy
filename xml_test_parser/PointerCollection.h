#pragma once
#include <list>

using namespace std;

class DataDescription
    {
    public:
        DataDescription(void);
        ~DataDescription(void);

        std::string toString();
    };

class Endpoint
    {
    public:
        Endpoint(void);
        ~Endpoint(void);

        std::string toString();
    };

class DataPointer
    {
    public:
        DataDescription *dd;
        list<Endpoint> *ep;
        DataPointer(void);
        ~DataPointer(void);
        
        std::string toString();
    };

class PointerCollection
    {
    public:
        list<DataPointer> *dp;

        PointerCollection(void);
        ~PointerCollection(void);
        
        std::string toString();
    };


