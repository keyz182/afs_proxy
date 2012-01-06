#include "HTTPUtils.h"

#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/MediaType.h"
#include "Poco/StreamCopier.h"
#include "Poco/Path.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"

using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPMessage;
using Poco::Net::MediaType;
using Poco::StreamCopier;
using Poco::Path;
using Poco::URI;
using Poco::Exception;

HTTPUtils::HTTPUtils(void)
{
}


HTTPUtils::~HTTPUtils(void)
{
}


std::string HTTPUtils::get(std::string url, long start, long end)
{
    std::string out;
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
            ///TODO: range
        }

        session.sendRequest(req);
        HTTPResponse res;
        std::istream& rs = session.receiveResponse(res);
        std::cout << res.getStatus() << " " << res.getReason() << std::endl;

        StreamCopier::copyToString(rs,out);
    }
    catch (Exception& exc)
    {
        std::cerr << exc.displayText() << std::endl;
        return "-1";
    }

    return out;
}

