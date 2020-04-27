//
// Created by Oliver Johnstone on 19/04/2020.
//

#ifndef HTTPSERV_STREAM_MANAGER_H
#define HTTPSERV_STREAM_MANAGER_H

#include "../v1/httpv_1.h"
#include "httpv_2.h"
#include <map>

namespace HTTPServ {
    class StreamManager {
        private:
            enum FrameType {DATA, HEADERS, PRIORITY, RST_STREAM, SETTINGS, PUSH_PROMISE, PING, GOAWAY, WINDOW_UPDATE, CONTINUATION};
            inline static const std::unordered_map<unsigned char, FrameType> FrameTypeMap = {
                {0, FrameType::DATA}, {1, FrameType::HEADERS}, {2, FrameType::PRIORITY},
                {3, FrameType::RST_STREAM}, {4, FrameType::SETTINGS}, {5, FrameType::PUSH_PROMISE},
                {6, FrameType::PING}, {7, FrameType::GOAWAY}, {8, FrameType::WINDOW_UPDATE},
                {9, FrameType::CONTINUATION}
            };

            typedef struct {
                bool END_STREAM; // bit 0
                bool ACK; // bit 0 - Mutually exclusive with END_STREAM
                bool END_HEADERS; // bit 2
                bool PADDED; // bit 3
                bool PRIORITY; // bit 5
            } FrameFlags;

            typedef struct {
                uint8_t length;
                FrameType type;
                FrameFlags flags;
                uint8_t streamId;
            } FrameHeader;

            HTTPV1 *httpV1 = nullptr;

            FrameHeader readFrameHeader(io::stream<HTTPServ::InSocketStream> &in);
            void readPreface(io::stream<HTTPServ::InSocketStream> &in);
            void readSettings(io::stream<HTTPServ::InSocketStream> &stream);
            void parseFrameFlags(FrameHeader &header, uint8_t flags);
            bool validateFrame(const FrameHeader &header);

        public:
            StreamManager(HTTPV1 *httpV1);
            StreamManager(io::stream<InSocketStream> &in, io::stream<OutSocketStream> &out);
            HTTPV2 *next();
            virtual ~StreamManager();
            void sendSettings(io::stream<HTTPServ::OutSocketStream> &stream);
    };
}

#endif //HTTPSERV_STREAM_MANAGER_H
