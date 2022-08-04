cmake_minimum_required(VERSION 3.22)
project(qraft_exchange_api)
set(CMAKE_CXX_STANDARD 17)

SET(INCDIR ./include)
include_directories(${INCDIR})
link_directories(/usr/local/lib)

###################################################################
# OS Config
if(APPLE)
    message(STATUS ">>> OS: APPLE")
elseif(LINUX OR UNIX)
    message(STATUS ">>> OS: LINUX OR UNIX")
    set(CMAKE_BUILD_TYPE Release)
elseif(WIN32)
    message(STATUS ">>> OS: WINDOWS")
    set(CMAKE_CXX_FLAGS "-fexec-charset=CP949")
endif()
###################################################################


# Compile and link
# Build the core library and executable
set(SOURCE_FILES
        main.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
add_subdirectory(unittest)