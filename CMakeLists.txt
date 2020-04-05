cmake_minimum_required(VERSION 3.15)
project(httpserv)

set(CMAKE_CXX_STANDARD 17)
set(BOOST_ROOT /usr/local/boost_1_72_0)

find_package(Boost)

if(NOT Boost_FOUND)
  message(FATAL_ERROR "Could not find boost!")
endif()

include_directories(
  include
  /usr/local/boost_1_72_0
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
  src/router/router.cpp)

add_executable(example
  example/src/example.cpp
  include/server/http.h)

target_link_libraries(example
  httpserv
)

target_link_libraries(httpserv
  ${Boost_LIBRARIES})