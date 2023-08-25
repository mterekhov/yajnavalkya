cmake_minimum_required(VERSION 3.19)

set(PROJECT_NAME yajnavalkya)

include_directories(third/CPP-SMTPClient-library/src)
include_directories(/opt/homebrew/Cellar/openssl@1.1/1.1.1u/include)

set(OPENSSL_ROOT_DIR /opt/homebrew/Cellar/openssl@1.1/1.1.1u)
find_package(OpenSSL REQUIRED)
message("OpenSSL include dir: ${OPENSSL_INCLUDE_DIR}")
message("OpenSSL libraries: ${OPENSSL_LIBRARIES}")
message("OpenSSL libraries2: ${OPENSSL_SSL_LIBRARIES}")

project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 20)
include(GNUInstallDirs)

add_executable(${PROJECT_NAME} yajnavalkya.cpp)

add_subdirectory(third/CPP-SMTPClient-library)

#target_include_directories(${PROJECT_NAME} PUBLIC ${OPENSSL_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC smtpclient ${OPENSSL_LIBRARIES} ${OPENSSL_SSL_LIBRARIES})
