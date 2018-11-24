cmake_minimum_required(VERSION 3.6)
project(demo-plugin)

# user config config file
include("${CMAKE_SOURCE_DIR}/user-config.cmake")

if(WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 8)
    set (OPENSSL_DIR "${SIELO_DIR}\\third-party\\openssl\\windows\\x64\\")
elseif(WIN32)
    set (OPENSSL_DIR "${SIELO_DIR}\\third-party\\openssl\\windows\\x86\\")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set (OPENSSL_DIR "${SIELO_DIR}/third-party/openssl/macos/")
endif()

include_directories(${SIELO_DIR}/Core)
include_directories(${CMAKE_SOURCE_DIR}/includes)

link_directories(${SIELO_LIBS_DIR})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

file(
    GLOB_RECURSE
    SOURCE_FILES
    includes/*.hpp
    sources/*.cpp
)

file(
	GLOB_RECURSE
	QRC_FILES
	*.qrc
)

find_package(OpenSSL 1.1.0 REQUIRED)
find_package(Qt5Widgets 5.11.2 REQUIRED)
find_package(Qt5WebEngine 5.11.2 REQUIRED)
find_package(Qt5Sql 5.11.2 REQUIRED)
find_package(Qt5WebEngineWidgets 5.11.2 REQUIRED)

set(SIELO_LIBS Core ${OPENSSL_LIBRARIES} Qt5::Widgets Qt5::WebEngine Qt5::Sql Qt5::WebEngineWidgets)
if(WIN32)
    set(SIELO_LIBS ${SIELO_LIBS} dwmapi uxtheme)
endif()

add_library(demo-plugin MODULE ${SOURCE_FILES} ${QRC_FILES})

target_link_libraries(demo-plugin PRIVATE ${SIELO_LIBS})