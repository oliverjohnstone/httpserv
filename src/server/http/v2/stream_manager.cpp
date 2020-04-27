//
// Created by Oliver Johnstone on 19/04/2020.
//

#include <iostream>
#include "stream_manager.h"

HTTPServ::StreamManager::StreamManager(HTTPServ::HTTPV1 *httpV1) : httpV1(httpV1) {
    // TODO - Upgrade connection, and create new v2 proto, pushing to stream
    // TODO - Setup stream manager in separate thread
}

HTTPServ::StreamManager::StreamManager(io::stream<HTTPServ::InSocketStream> &in, io::stream<HTTPServ::OutSocketStream> &out) {
    readPreface(in);
    sendSettings(out);
    readSettings(in);
    // TODO - Try to parse the request as though it was initialised as a v2 request
}

HTTPServ::HTTPV2 *HTTPServ::StreamManager::next() {
    // TODO - Return next highest priority request from request tree
    return nullptr;
}

HTTPServ::StreamManager::~StreamManager() {
    delete httpV1;
}

void HTTPServ::StreamManager::readPreface(io::stream<HTTPServ::InSocketStream> &in) {
    auto expectedPreface = "PRI * HTTP/2.0\r\n\r\nSM\r\n\r\n";
    auto *ptr = expectedPreface;

    while (*ptr != '\0' && in.get() == *ptr && !in.eof() && !in.fail()) {
        ptr++;
    }

    if (*ptr != '\0') {
        // TODO - send proto error
        // TODO - Throw a better error
        throw std::logic_error("Unexpected preface from client.");
    }
}

void HTTPServ::StreamManager::readSettings(io::stream<HTTPServ::InSocketStream> &stream) {
    auto frameHeader = readFrameHeader(stream);
    if (frameHeader.length) {
        // TODO - Read and parse settings data
    }

    FrameHeader ackSettings = {.length=0, .flags={.ACK=true}, .type=FrameType::SETTINGS, .streamId=0};
    // TODO - Send Ack settings
}

HTTPServ::StreamManager::FrameHeader HTTPServ::StreamManager::readFrameHeader(io::stream<HTTPServ::InSocketStream> &in) {
    uint8_t buf[9];
    uint8_t *ptr = buf;
    in.read(reinterpret_cast<char *>(buf), 9);

    if (in.eof() || in.fail()) {
        // TODO - Throw better error
        // TODO - Send proto error
        throw std::logic_error("Unexpected eof / failed read");
    }

    FrameHeader header = {.length=0, .streamId=0};

    // Parse frame length, first 24 bits
    for (auto i = 0, n = 16; i < 3; i++, n -= 8) {
        header.length &= ((0 & *ptr++) << n);
    }

    auto type = *ptr++;
    try {
        // Frame type, next 8 bits
        header.type = FrameTypeMap.at(type);
    } catch (std::out_of_range &e) {
        header.type = (FrameType)type;
    }

    // Frame flags, next 8 bits
    parseFrameFlags(header, *ptr++);

    // Parse stream id, next 32 bits where the first bit (R) should always be 0
    for (auto i = 0, n = 24; i < 3; i++, n -= 8) {
        header.streamId &= ((0 & *ptr++) << n);
    }

    if (!validateFrame(header)) {
        // TODO - Throw better error
        // TODO - Send proto error
        throw std::logic_error("Invalid frame");
    }

    return header;
}

bool HTTPServ::StreamManager::validateFrame(const HTTPServ::StreamManager::FrameHeader &header) {
    if ((header.streamId >> 31) != 0) {
        // Reserved 1 bit field in stream id field
        return false;
    }

    if (header.streamId % 2) {
        // Streams initialized by client must be odd
        return false;
    }

    // Connection management frames should always be on stream id 0
    switch (header.type) {
        case FrameType::SETTINGS:
        case FrameType::PING:
        case FrameType::GOAWAY:
        case FrameType::WINDOW_UPDATE:
            if (header.streamId != 0) {
                return false;
            }
            break;

        case FrameType::DATA:
        case FrameType::HEADERS:
        case FrameType::PRIORITY:
        case FrameType::RST_STREAM:
        case FrameType::PUSH_PROMISE:
        case FrameType::CONTINUATION:
            if (header.streamId == 0) {
                return false;
            }
            break;
    }

    switch (header.type) {
        case FrameType::WINDOW_UPDATE:
        case FrameType::PRIORITY:
        case FrameType::RST_STREAM:
        case FrameType::GOAWAY:
            if (header.flags.PRIORITY || header.flags.PADDED || header.flags.END_HEADERS || header.flags.END_STREAM || header.flags.ACK) {
                // No flags supported
                return false;
            }
    }

    return true;
}

void HTTPServ::StreamManager::parseFrameFlags(HTTPServ::StreamManager::FrameHeader &header, uint8_t flags) {
    if (flags & (uint8_t)1) {
        if (header.type == FrameType::PING) {
            header.flags.ACK = true;
        } else {
            header.flags.END_STREAM = true;
        }
    }

    if (flags & (uint8_t)4) {
        header.flags.END_HEADERS = true;
    }

    if (flags & (uint8_t)8) {
        header.flags.PADDED = true;
    }

    if (flags & (uint8_t)32) {
        header.flags.PRIORITY = true;
    }
}

void HTTPServ::StreamManager::sendSettings(io::stream<HTTPServ::OutSocketStream> &stream) {
    FrameHeader header = {};
}
