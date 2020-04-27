cmake_minimum_required(VERSION 3.15)
project(httpserv)

set(CMAKE_CXX_STANDARD 20)
set(BOOST_ROOT ~/boost-lib)
set(OPENSSL_ROOT ~/openssl)
set(CMAKE_FIND_LIBRARY_SUFFIXES .a)

find_package(Boost 1.72.0 COMPONENTS regex)
find_package(nlohmann_json 3.2.0 REQUIRED)

link_directories(
  lib/path_matcher
  ${OPENSSL_ROOT}/lib
)

if(NOT Boost_FOUND)
  message(FATAL_ERROR "Could not find boost!")
endif()

include_directories(
  include
  ${BOOST_ROOT}/include
  ${OPENSSL_ROOT}/include
  lib
)

add_library(httpserv
  src/server/server.cpp
  src/logger/logger.cpp
  src/server/socket/socket.cpp
  src/server/socket/stream.cpp
  src/server/connection.cpp
  src/server/http/request.cpp
  src/server/http/response.cpp
  include/server/error.h
  include/server/http.h
  src/server/http.cpp
  src/router/router.cpp
  src/server/http/http_stream.cpp
  src/server/http/http_stream.h
  src/server/http/v1/httpv_1.cpp
  src/server/http/v1/httpv_1.h
  src/server/http/version.cpp
  src/server/http/version.h
  src/server/http/v2/httpv_2.cpp
  src/server/http/v2/httpv_2.h
  src/server/http/v2/v_2_stream.cpp src/server/http/v2/v_2_stream.h src/server/http/v2/stream_manager.cpp src/server/http/v2/stream_manager.h)

add_executable(example
  example/src/example.cpp
  include/server/http.h include/server/tls.h)

target_link_libraries(example
  httpserv
)

target_link_libraries(httpserv
  ${Boost_LIBRARIES}
  nlohmann_json::nlohmann_json
  path_matcher
  ssl
  crypto
)