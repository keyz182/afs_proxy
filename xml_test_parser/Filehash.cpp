#include "Filehash.h"

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


Filehash::Filehash(const char* xml)
{
    this->seg = new list<Segment>();
    Poco::XML::DOMParser parser;
    Poco::AutoPtr<Poco::XML::Document> pDoc = parser.parseString(xml);

    Poco::XML::NodeIterator it(pDoc, Poco::XML::NodeFilter::SHOW_ALL);
    Poco::XML::Node* pNode = it.nextNode();
    pNode = it.nextNode();

    if(pNode->nodeName() == "FileHash"){
        Poco::XML::Node* nSeg = pNode->firstChild();

        while(nSeg){
            if(nSeg->nodeName() == "hash"){
                this->hash = nSeg->firstChild()->nodeValue();
            }else if(nSeg->nodeName() == "size"){
                this->size = atol(nSeg->firstChild()->nodeValue().c_str());
            }else if(nSeg->nodeName() == "Segment"){
                Segment *s = new Segment(nSeg);
                this->seg->push_back(*s);
            }
            nSeg = nSeg->nextSibling();
        } 
    }
}

Filehash::~Filehash(void)
{
}

string Filehash::toString()
{
    stringstream ss;
    ss << "Hash : " << this->hash << "\n";
    ss << "size : " << this->size << "\n";

    list<Segment>::iterator i;

    for(i = this->seg->begin(); i != this->seg->end(); i++){
        ss << (*i).toString();
    }

    return ss.str();
}



Segment::Segment(Poco::XML::Node* pNode)
{
    Poco::XML::Node* seg = pNode->firstChild();

    while(seg){
        if(seg->nodeName() == "hash"){
            this->hash = seg->firstChild()->nodeValue(); 
        }
        else if(seg->nodeName() == "start"){
            this->start = atol(seg->firstChild()->nodeValue().c_str()); 
        }
        else if(seg->nodeName() == "end"){
            this->end = atol(seg->firstChild()->nodeValue().c_str());
        }
        seg = seg->nextSibling();
    }
}

Segment::~Segment(void)
{
}

string Segment::toString()
{
    stringstream ss;
    ss << "Hash  : " << this->hash << "\n";
    ss << "Start : " << this->start << "\n";
    ss << "End   : " << this->end << "\n";

    return ss.str();
}