#include "PointerCollection.h"

#include <sstream>

#include "Poco/DOM/DOMParser.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/NodeIterator.h"
#include "Poco/DOM/NodeFilter.h"
#include "Poco/DOM/AutoPtr.h"
#include "Poco/SAX/InputSource.h"



using Poco::XML::DOMParser;
using Poco::XML::InputSource;
using Poco::XML::Document;
using Poco::XML::NodeIterator;
using Poco::XML::NodeFilter;
using Poco::XML::Node;
using Poco::XML::AutoPtr;


PointerCollection::PointerCollection(const char* xml)
{
    this->dp = new list<DataPointer>();
    Poco::XML::DOMParser parser;
    Poco::AutoPtr<Poco::XML::Document> pDoc = parser.parseString(xml);

    Poco::XML::NodeIterator it(pDoc, Poco::XML::NodeFilter::SHOW_ALL);
    Poco::XML::Node* pNode = it.nextNode();
    pNode = it.nextNode();

    if(pNode->nodeName() == "PointerCollection"){
        Poco::XML::Node* nPC = pNode->firstChild();

        while(nPC){
            if(nPC->nodeName() == "DataPointer"){
                DataPointer *d = new DataPointer(nPC);
                this->dp->push_back(*d);
            }
            nPC = nPC->nextSibling();
        } 
    }
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


DataPointer::DataPointer(Poco::XML::Node* pNode)
{
    Poco::XML::Node* nDP = pNode->firstChild();
    this->ep = new list<Endpoint>();
    //this->dd = new DataDescription();

    while(nDP){
        if(nDP->nodeName() == "DataDescription"){
            dd = new DataDescription(nDP);
        }else if(nDP->nodeName() == "Endpoint"){
            Endpoint *ep = new Endpoint(nDP);
            this->ep->push_back(*ep);
        }
        nDP =nDP->nextSibling();
    }
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

DataDescription::DataDescription(Poco::XML::Node* pNode)
{
    Poco::XML::Node* nDD = pNode->firstChild();

    while(nDD){
        if(nDD->nodeName() == "id"){
            this->id = nDD->firstChild()->nodeValue(); 
        }
        else if(nDD->nodeName() == "name"){
            this->name = nDD->firstChild()->nodeValue(); 
        }
        else if(nDD->nodeName() == "project"){
            this->project = nDD->firstChild()->nodeValue(); 
        }
        else if(nDD->nodeName() == "description"){
            this->description = nDD->firstChild()->nodeValue(); 
        }
        nDD = nDD->nextSibling();
    }
}
DataDescription::~DataDescription(void)
{
}

std::string DataDescription::toString()
{
    std::stringstream ss;
    ss << "ID         :  " << this->id << "\n";
    ss << "Name       : " << this->name << "\n";
    ss << "Project    :  " << this->project << "\n";
    ss << "Description: " << this->description << "\n";
    return ss.str();
}

Endpoint::Endpoint(Poco::XML::Node*pNode)
{
    Poco::XML::Node* nEE = pNode->firstChild();

    while(nEE){
        if(nEE->nodeName() == "url"){
            this->url = nEE->firstChild()->nodeValue(); 
        }
        else if(nEE->nodeName() == "meta"){
            this->meta = nEE->firstChild()->nodeValue(); 
        }
        nEE = nEE->nextSibling();
    }

}

Endpoint::~Endpoint(void)
{
}

std::string Endpoint::toString()
{
    std::stringstream ss;
    ss << "URL:  " << this->url << "\n";
    ss << "META: " << this->meta << "\n";
    return ss.str();
}