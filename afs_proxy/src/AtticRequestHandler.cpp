#include "AtticRequestHandler.h"

#include "Poco/Exception.h"
#include "Poco/ThreadPool.h"
#include "Poco/StreamCopier.h"
#include "Poco/Random.h"
#include "Poco/Path.h"
#include "Poco/File.h"
#include <algorithm>
#include <sstream>
#include "PointerCollection.h"
#include "Filehash.h"
#include "Chunk.h"
#include "HTTPUtils.h"
#include <fstream>

#include "Poco/DigestStream.h"
#include "Poco/MD5Engine.h"

#include "Poco/Util/IniFileConfiguration.h"

using Poco::AutoPtr;
using Poco::Util::IniFileConfiguration;

using Poco::Exception;
using Poco::DigestOutputStream;
using Poco::DigestEngine;
using Poco::MD5Engine;

using Poco::Random;
using Poco::StreamCopier;
using Poco::Path;
using Poco::File;

AtticRequestHandler::AtticRequestHandler() 
{
}

void AtticRequestHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
{
    File ini("config.ini");

    std::string datalookup = "http://voldemort.cs.cf.ac.uk:7048/dl/meta/pointer/";

    if(ini.exists()){
        AutoPtr<IniFileConfiguration> pConf(new IniFileConfiguration(ini.path()));
        datalookup = pConf->getString("afsproxy.datalookup","http://voldemort.cs.cf.ac.uk:7048/dl/meta/pointer/");
    }

    Application& app = Application::instance();
    app.logger().information("Attic Request from " + request.clientAddress().toString());

    Path path(request.getURI());

    try
    {
        Filehash* f = 0;
        std::string pointer;
        std::vector<std::string> validEndpoints;

        pointer = HTTPUtils::get(datalookup);

        if(pointer == "-1"){
            return ;
        }

        PointerCollection* p = new PointerCollection(pointer.c_str());

        list<DataPointer>::iterator i;

        for(i = p->dp->begin(); i != p->dp->end(); i++){

            if((*i).dd->id.compare(path.getFileName())==0){
                list<Endpoint>::iterator e;

                for(e = (*i).ep->begin(); e != (*i).ep->end(); e++){
                    std::stringstream ss;
                    ss << (*e).meta << "?filehash=" << (*i).dd->id;
                    std::string out = HTTPUtils::get(ss.str());

                    if(out != "-1"){
                        f = new Filehash(out.c_str());
                        std::cout << f->toString();
                        (*e).accessible = true;
                        validEndpoints.push_back((*e).url);
                    }
                    std::cout << "\n";
                }
            }
        }

        int noValidEndpoints = (int) validEndpoints.size();

        if((f==0) || (noValidEndpoints < 1)){
            response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
            response.setReason("ID Not found");
            response.send() << "ID Not Found";
            return;
        }


        list<Segment>::iterator it;
        list<Chunk> chunks;

        for(it = f->seg->begin(); it != f->seg->end(); it++)
        {
            chunks.push_back(Chunk(validEndpoints,(*it).hash,(*it).start,(*it).end));
        }

        Poco::ThreadPool tp(2,8);

        list<Chunk>::iterator cit;

        for(cit = chunks.begin(); cit != chunks.end(); cit++)
        {
            while(tp.available() <= 0)
            {
                tp.collect();
            }

            tp.start((*cit));
        }

        tp.joinAll();

        bool chunksFailed = true;

        while(chunksFailed){
            chunksFailed = false;
            for(cit = chunks.begin(); cit != chunks.end(); cit++)
            {
                if((*cit).failed){
                    chunksFailed = true;
                    while(tp.available() <= 0)
                    {
                        tp.collect();
                    }

                    tp.start((*cit));
                }
            }
            tp.joinAll();
        }

        Poco::TemporaryFile tmp;

        std::ofstream ostr(tmp.path().c_str(), std::ios::binary);

        for(cit = chunks.begin(); cit != chunks.end(); cit++)
        {
            std::ifstream istr((*cit).tmp.path().c_str(),std::ios::binary);
            StreamCopier::copyStream(istr,ostr);
            istr.close();
        }

        ostr.close();

        MD5Engine md5;
        DigestOutputStream md5str(md5);

        std::ifstream istr(tmp.path().c_str(), std::ios::binary);

        StreamCopier::copyStream(istr,md5str);

        md5str.flush();


        const DigestEngine::Digest& digest = md5.digest(); // obtain result
        std::string result = DigestEngine::digestToHex(digest);

        //Workaround, attic seems to be randomly dropping zeros from md5 hashes.

        result.erase(remove(result.begin(),result.end(),'0'),result.end());
        f->hash.erase(std::remove(f->hash.begin(),f->hash.end(),'0'),f->hash.end());

        std::cout << "\n" << result << "\n" << f->hash << "\n";

        std::cout << p->toString();

        istr.close();
        delete istr;

        if(f->hash.compare(result) !=0){
            response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            response.setReason("File hash didn't match");
            response.send() << "Failed";
        }else{
            std::ifstream is(tmp.path().c_str(), std::ios::binary);
            StreamCopier::copyStream(is,response.send());
        }
        return ;

    }
    catch (Exception& exc)
    {
        std::cerr << exc.displayText() << std::endl;
        return;
    }
}