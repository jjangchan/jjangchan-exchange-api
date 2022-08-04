project(unittest)
add_subdirectory(lib)
set(CMAKE_CXX_STANDARD 17)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(${PROJECT_NAME} GetPathTest.cpp)
target_link_libraries(${PROJECT_NAME} gtest gtest_main)