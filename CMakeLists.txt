cmake_minimum_required(VERSION 3.19)

set(PROJECT_NAME yajnavalkya)


if(APPLE)
	set(OPENSSL_ROOT_DIR /opt/homebrew/Cellar/openssl@1.1/1.1.1u/)
 	include_directories(/opt/homebrew/Cellar/openssl@1.1/1.1.1u/include)

	include_directories(/opt/homebrew/opt/tesseract/include)
	include_directories(/opt/homebrew/opt/leptonica/include)
	
	set(CMAKE_PREFIX_PATH /opt/homebrew/opt/tesseract/lib)
	set(CMAKE_PREFIX_PATH /opt/homebrew/opt/leptonica/lib)
endif()

if(UNIX AND NOT APPLE)
	set(OPENSSL_ROOT_DIR /usr/local)
	find_package(PkgConfig REQUIRED)

	pkg_check_modules(Tesseract REQUIRED IMPORTED_TARGET tesseract lept)
	add_library(Tesseract::libtesseract ALIAS PkgConfig::Tesseract)
endif()

find_package(OpenSSL REQUIRED)

project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 20)
include(GNUInstallDirs)

include_directories(/usr/local/include)

add_executable(${PROJECT_NAME} yajnavalkya.cpp YVBLSRussiaPortugalAPI.cpp YVHTTPRequestService.cpp YVIMAPClient.cpp YVSSLSocket.cpp YVTools.cpp YVTelegramBotAPI.cpp)

if(APPLE)
	find_library(TESSERACT_LIB tesseract lib)
	find_library(LEPTONICA_LIB lept lib)
	target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${OPENSSL_LIBRARIES} ${OPENSSL_SSL_LIBRARIES} ${TESSERACT_LIB} ${LEPTONICA_LIB})
endif()

if(UNIX AND NOT APPLE)
	target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${OPENSSL_LIBRARIES} ${OPENSSL_SSL_LIBRARIES} Tesseract::libtesseract)
endif()
