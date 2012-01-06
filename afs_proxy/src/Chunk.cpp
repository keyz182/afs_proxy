#include "Chunk.h"
#include <sstream>

#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/MediaType.h"
#include "Poco/StreamCopier.h"
#include "Poco/Path.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include <fstream>

#include "Poco/DigestStream.h"
#include "Poco/MD5Engine.h"
using Poco::DigestOutputStream;
using Poco::DigestEngine;
using Poco::MD5Engine;

using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPMessage;
using Poco::Net::MediaType;
using Poco::StreamCopier;
using Poco::Path;
using Poco::URI;
using Poco::Exception;
using Poco::TemporaryFile;

Chunk::Chunk(std::vector<std::string> endpoints, std::string hash, long start, long end)
{      
    this->endpoints = endpoints;
    this->hash = hash;
    this->start = start;
    this->end = end;
}


Chunk::~Chunk(void)
{
}


void Chunk::run()
{
    std::vector<std::string>::iterator it;

    for(it = endpoints.begin(); it != endpoints.end(); it++)
    {
        try{
            getData((*it),start,end);
            if(dataHash.compare(hash)!= 0){
                this->failed = true;
                std::cout << "Hash doesn't match";
                continue;
            }
            this->failed = false;
            return;
        }catch(Exception& exc){
            std::cout << "Endpoint failed: " << (*it);
            this->failed = true;
            continue;
        }
    }

    tmp.remove();
    this->failed = false;
}

void Chunk::getData(std::string url, long start, long end)
{
    if(tmp.exists()){
        tmp.remove();
    }

    try
    {
        URI uri(url);
        //URI uri("http://localhost:8080");
        std::string path(uri.getPathAndQuery());
        if (path.empty()) path = "/";

        HTTPClientSession session(uri.getHost(), uri.getPort());
        HTTPRequest req(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
        req.add("Accept","text/xml");

        if((start>=0)&&(end>=0)){
            std::stringstream ss;
            ss << "bytes=" << start << "-" << end;
            req.add("Range",ss.str());
        }

        session.sendRequest(req);
        HTTPResponse res;
        std::istream& rs = session.receiveResponse(res);
        std::cout << res.getStatus() << " " << res.getReason() << std::endl;

        std::ofstream ostr(this->tmp.path().c_str(), std::ios::binary);

        StreamCopier::copyStream(rs,ostr);

        ostr.close();

        MD5Engine md5;
        DigestOutputStream md5str(md5);

        std::ifstream istr(this->tmp.path().c_str(), std::ios::binary);

        StreamCopier::copyStream(istr,md5str);

        istr.close();

        md5str.flush();


        const DigestEngine::Digest& digest = md5.digest(); // obtain result
        dataHash = DigestEngine::digestToHex(digest);

        //Workaround, attic seems to be randomly dropping zeros from md5 hashes.
        dataHash.erase(std::remove(dataHash.begin(),dataHash.end(),'0'),dataHash.end());
        this->hash.erase(std::remove(this->hash.begin(),this->hash.end(),'0'),this->hash.end());

        std::cout << "\n" << dataHash << "\n" << this->hash;

        std::cout << "\n";

        //rs.read(data,end-start);

    }
    catch (Exception& exc)
    {
        std::cerr << exc.name() << ", " << exc.displayText() << std::endl;

        exc.rethrow();
        /*
        data = 0;
        return;         
        */
    }

    return;
}