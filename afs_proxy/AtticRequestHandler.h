#pragma once
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Util/Application.h"

using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Util::Application;

class AtticRequestHandler: public HTTPRequestHandler
    /// Return a HTML document with the current date and time.
{
public:
    AtticRequestHandler();

    void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response);
};
