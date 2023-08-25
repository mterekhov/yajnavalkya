cmake_minimum_required(VERSION 3.19)

set(PROJECT_NAME yajnavalkya)


if(APPLE)
	set(OPENSSL_ROOT_DIR /opt/homebrew/Cellar/openssl@1.1/1.1.1u/)
 	include_directories(/opt/homebrew/Cellar/openssl@1.1/1.1.1u/include)
endif()

if(UNIX AND NOT APPLE)
	set(OPENSSL_ROOT_DIR /usr/local)
endif()

find_package(OpenSSL REQUIRED)

project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 20)
include(GNUInstallDirs)

add_executable(${PROJECT_NAME} yajnavalkya.cpp)
include_directories(third/CPP-SMTPClient-library/src)
add_subdirectory(third/CPP-SMTPClient-library)

target_link_libraries(${PROJECT_NAME} LINK_PUBLIC smtpclient ${OPENSSL_LIBRARIES} ${OPENSSL_SSL_LIBRARIES})
