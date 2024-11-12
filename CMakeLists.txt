cmake_minimum_required(VERSION 3.10)
project(crackit)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Point to the SDL2 directory
list(APPEND CMAKE_MODULE_PATH "SDL2-devel-2.28.2-mingw/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/SDL2_ttf-devel-version-mingw/cmake")

set(SDL2_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SDL2-devel-2.28.2-mingw/cmake")
set(SDL2_ttf_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SDL2_ttf-2.20.2/cmake")

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

message(STATUS "SDL2_ttf include directories: ${SDL2_ttf_INCLUDE_DIRS}")

#Add all the files to the exec
file(GLOB SOURCES 
    "src/*.cpp" "include/*.hpp" 
    "src/Utils/*.cpp" "include/Utils/*.hpp" 
    "src/Graphics/*.cpp" "include/Graphics/*.hpp"
    "src/Shapes/*.cpp" "include/Shapes/*.hpp"
    "src/Crackit/*.cpp" "include/Crackit/*.hpp"
    "src/Scenes/*.cpp" "include/Scenes/*.hpp"
    "src/Input/*.cpp" "include/Input/*.hpp"
)


# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Copy the assets folder to the build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/src/Assets"  # Specify the path to your assets folder
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/Assets)


# Include directories for all targets
target_include_directories(${PROJECT_NAME} PUBLIC
    ${SDL2_ttf_INCLUDE_DIRS}  # Include SDL2_ttf's include directory
    ${SDL2_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/Graphics
    ${CMAKE_SOURCE_DIR}/include/Input
    ${CMAKE_SOURCE_DIR}/include/Utils
    ${CMAKE_SOURCE_DIR}/include/Shapes
    ${CMAKE_SOURCE_DIR}/include/Crackit
    ${CMAKE_SOURCE_DIR}/include/Scenes
)

# Link SDL2
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_ttf_LIBRARIES} SDL2_ttf)  # Link against SDL2_ttf

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

# Install SDL2 and SDL2_ttf DLLs (specify the correct paths)
install(FILES
    "C:/Libraries/SDL2/x86_64-w64-mingw32/bin/SDL2.dll"
    "C:/Libraries/SDL2/x86_64-w64-mingw32/bin/SDL2_ttf.dll"
    DESTINATION bin
)
# Install any game assets you have. Let's assume they are in an "assets" folder.
install(DIRECTORY src/Assets/ DESTINATION bin/Assets)

set(CMAKE_CXX_FLAGS_RELEASE "-static-libgcc -static-libstdc++ ${CMAKE_CXX_FLAGS_RELEASE}")
set(CMAKE_CXX_FLAGS_DEBUG "-static-libgcc -static-libstdc++ ${CMAKE_CXX_FLAGS_DEBUG}")

set(CPACK_PACKAGE_NAME "Crackit")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_GENERATOR "ZIP")
include(CPack)