//
// Created by Oliver Johnstone on 17/04/2020.
//

#include "http_stream.h"
#include "v1/httpv_1.h"

HTTPServ::HTTPStream::HTTPStream(io::stream<InSocketStream> &inStream, io::stream<OutSocketStream> &outStream,
    int maxRequests, int connectionTimeout) : inStream(inStream), outStream(outStream), maxRequests(maxRequests),
    connectionTimeout(connectionTimeout) {

}

HTTPServ::HTTPVersion* HTTPServ::HTTPStream::next() {
    HTTPVersion* versionImpl = nullptr;

    if (!v2StreamManager && inStream.component()->isH2Socket()) {
        // Start of a new http/2 stream
        v2StreamManager = new StreamManager(inStream, outStream);
        return v2StreamManager->next();
    }

    if (v2StreamManager) {
        return v2StreamManager->next();
    }

    // Max request count only applies to v1 requests
    if (currentRequest >= maxRequests) {
        return nullptr;
    }

    auto v1 = new HTTPV1(inStream, outStream);
    if (v1->canUpgrade()) {
        v2StreamManager = new StreamManager(v1);
        versionImpl = v2StreamManager->next();
    } else {
        versionImpl = v1;
    }

    currentRequest++;
    return versionImpl;
}

HTTPServ::HTTPStream::~HTTPStream() {
    delete v2StreamManager;
}
