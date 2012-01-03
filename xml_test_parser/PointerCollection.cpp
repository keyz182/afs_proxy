#include "PointerCollection.h"

#include <sstream>

PointerCollection::PointerCollection(void)
    {
        this->dp = new list<DataPointer>();
    }

PointerCollection::~PointerCollection(void)
    {
    }

std::string PointerCollection::toString()
    {
        std::stringstream ss;
        list<DataPointer>::iterator i;
        for (i=dp->begin();i!=dp->end();i++)
            {
            ss << (*i).toString();
            }
        return ss.str();
    }


DataPointer::DataPointer(void)
    {
        this->ep = new list<Endpoint>();
        this->dd = new DataDescription();
    }

DataPointer::~DataPointer(void)
    {
    }

std::string DataPointer::toString()
    {
        std::stringstream ss;
        ss << dd->toString();
        list<Endpoint>::iterator i;
        for (i=ep->begin();i!=ep->end();i++)
            {
            ss << (*i).toString();
            }
        return ss.str();
    }

DataDescription::DataDescription(void)
    {
    }

DataDescription::~DataDescription(void)
    {
    }

std::string DataDescription::toString()
    {
        return "abc";
    }

Endpoint::Endpoint(void)
    {
    }

Endpoint::~Endpoint(void)
    {
    }

std::string Endpoint::toString()
    {
        return "def";
    }