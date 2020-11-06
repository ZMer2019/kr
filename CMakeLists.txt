cmake_minimum_required(VERSION 3.15)
project(kr)

set(CMAKE_CXX_STANDARD 11)
set(CryptoPP_DIR /usr/local/lib)
find_package(Threads REQUIRED)

set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${Protobuf_VERSION}")
set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
SET(_REFLECTION gRPC::grpc++_reflection)

find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")

set(_GRPC_GRPCPP gRPC::grpc++)

#include_directories(/usr/local/include)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/http_server)
link_directories(/usr/local/lib)

get_filename_component(auth_message "protos/auth_message.proto" ABSOLUTE)
message("${auth_message}")
get_filename_component(protos_path "${auth_message}" DIRECTORY)
message("${protos_path}")
set(proto_auth_message_srcs "${CMAKE_CURRENT_BINARY_DIR}/auth_message.pb.cc")
set(proto_auth_message_hdrs "${CMAKE_CURRENT_BINARY_DIR}/auth_message.pb.h")
add_custom_command(
        OUTPUT "${proto_auth_message_srcs}"
        "${proto_auth_message_hdrs}"
        COMMAND protoc
        ARGS -I "${protos_path}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}" "${auth_message}"
        DEPENDS "${auth_message}"
)

get_filename_component(issuecert "protos/issuecert.proto" ABSOLUTE)
set(proto_issuecert_srcs "${CMAKE_CURRENT_BINARY_DIR}/issuecert.pb.cc")
set(proto_issuecert_hdrs "${CMAKE_CURRENT_BINARY_DIR}/issuecert.pb.h")
set(grpc_issuecert_srcs "${CMAKE_CURRENT_BINARY_DIR}/issuecert.grpc.pb.cc")
set(grpc_issuecert_hdrs "${CMAKE_CURRENT_BINARY_DIR}/issuecert.grpc.pb.h")
add_custom_command(
        OUTPUT ${proto_issuecert_srcs}
        ${proto_issuecert_hdrs}
        ${grpc_issuecert_srcs}
        ${grpc_issuecert_hdrs}
        COMMAND protoc
        ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${protos_path}"
        --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` ${issuecert}
        DEPENDS "${issuecert}"
)

add_executable(kr
        main.cpp
        ${proto_auth_message_srcs}
        ${proto_auth_message_hdrs}
        ${proto_issuecert_srcs}
        ${proto_issuecert_hdrs}
        ${grpc_issuecert_srcs}
        ${grpc_issuecert_hdrs}
        crypto.cpp
        crypto.h
        http_server/httplib.h
        http_server/http_server.cpp
        http_server/http_server.h
        grpc_client.cpp
        grpc_client.h
        cxxopts.hpp)

target_link_libraries(kr ${_PROTOBUF_LIBPROTOBUF} ${_REFLECTION} ${_GRPC_GRPCPP} cryptopp)