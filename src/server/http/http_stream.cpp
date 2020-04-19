//
// Created by Oliver Johnstone on 17/04/2020.
//

#include "http_stream.h"
#include "v1/httpv_1.h"
#include "v2/httpv_2.h"

HTTPServ::HTTPStream::HTTPStream(io::stream<InSocketStream> &inStream, io::stream<OutSocketStream> &outStream, int maxRequests, int connectionTimeout) :
    inStream(inStream), outStream(outStream), maxRequests(maxRequests), connectionTimeout(connectionTimeout) {
}

HTTPServ::HTTPVersion* HTTPServ::HTTPStream::next() {
    if (currentRequest >= maxRequests) {
        return nullptr;
    }

    HTTPVersion* versionImpl = nullptr;

    if (!version) {
        auto v1 = new HTTPV1(inStream, outStream);
        if (v1->canUpgrade()) {
            versionImpl = (new HTTPV2(v1))->upgrade();
            version = HTTP::VERSION_2;
        } else {
            versionImpl = v1;
            version = HTTP::VERSION_1_1;
        }
    } else {
        if (version == HTTP::VERSION_2) {
            // TODO - Create new filtered pipes and setup v2
            throw std::logic_error("Unimplemented");
        } else {
            versionImpl = new HTTPV1(inStream, outStream);
        }
    }

    currentRequest++;
    return versionImpl;
}
