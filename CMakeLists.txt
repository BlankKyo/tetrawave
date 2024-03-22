cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(tetrawave)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}) # Transfer from a pc to another
# Dependencies
set(RAYLIB_VERSION 4.5.0)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)
    FetchContent_Declare(
            raylib
            DOWNLOAD_EXTRACT_TIMESTAMP OFF
            URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif()
endif()

# Our Project

add_executable(${PROJECT_NAME} main.cpp
        colors.h
        colors.cpp
        game.h
        game.cpp
        grid.h
        grid.cpp
        blocks.cpp
        block.h
        block.cpp
        position.cpp
        position.h
        Menu/Background.h
        Menu/Background.cpp
        Button/Buttons.h
        Button/Buttons.cpp
        connection.cpp
        connection.h
        robot.hpp
        robot.cpp
)

#set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
find_package(SFML REQUIRED system window graphics network)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${SFML_LIBRARIES})
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/lib/sfml/include)

#copy dlls to build directory
if(WIN32)
    file(GLOB BINARY_DEP_DLLS "${SFML_INCLUDE_DIR}/../bin/*.dll")
    file(COPY ${BINARY_DEP_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
    file(GLOB MINGW_DEP_DLLS "C:/mingw64/bin/*.dll")
    file(COPY ${MINGW_DEP_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
endif()

