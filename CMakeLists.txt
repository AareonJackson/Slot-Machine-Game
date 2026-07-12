# Root CMakeLists.txt for Slot-Machine project
cmake_minimum_required(VERSION 3.15)
project(Slot-Machine VERSION 0.1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Options
option(BUILD_TESTS "Build test programs" ON)
option(BUILD_DOCS "Build documentation" OFF)
option(ENABLE_DEBUG_LOGS "Enable debug console logging" OFF)

# Output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include custom CMake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(FetchContent)

FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

# Check if SFML exists in the specified external directory
set(SFML_LOCAL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/sfml")
if(EXISTS "${SFML_LOCAL_PATH}")
    message(STATUS "Using local SFML from ${SFML_LOCAL_PATH}")

    if(EXISTS "${SFML_LOCAL_PATH}/lib/cmake/SFML/SFMLConfig.cmake")
        set(SFML_DIR "${SFML_LOCAL_PATH}/lib/cmake/SFML")
        find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)
    elseif(EXISTS "${SFML_LOCAL_PATH}/CMakeLists.txt")
        set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "Build the SFML examples" FORCE)
        set(SFML_BUILD_DOC OFF CACHE BOOL "Build the SFML documentation" FORCE)
        set(SFML_BUILD_NETWORK OFF CACHE BOOL "Build the SFML network module" FORCE)
        set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)

        add_subdirectory(${SFML_LOCAL_PATH} sfml_build)
    else()
        message(FATAL_ERROR "SFML directory exists but doesn't contain expected files. Please ensure SFML is properly placed in ${SFML_LOCAL_PATH}")
    endif()
else()
    message(STATUS "Local SFML not found at ${SFML_LOCAL_PATH}, downloading it...")

    include(FetchContent)
    FetchContent_Declare(
            SFML
            GIT_REPOSITORY https://github.com/SFML/SFML.git
            GIT_TAG 2.6.1
    )

    # Configure SFML build options
    set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "Build the SFML examples" FORCE)
    set(SFML_BUILD_DOC OFF CACHE BOOL "Build the SFML documentation" FORCE)
    set(SFML_BUILD_NETWORK OFF CACHE BOOL "Build the SFML network module" FORCE)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)

    # WORKAROUND FOR MACOS CMAKE CASE-SENSITIVITY BUG
    # SFML requests VORBIS but provides FindVorbis.cmake. CMake's strict casing rejects it.
    # We create a proxy file in an isolated directory to safely bypass this check.
    set(PROXY_MODULE_DIR "${CMAKE_BINARY_DIR}/cmake_proxy")
    file(MAKE_DIRECTORY "${PROXY_MODULE_DIR}")
    file(WRITE "${PROXY_MODULE_DIR}/FindVORBIS.cmake" "include(\"${CMAKE_BINARY_DIR}/_deps/sfml-src/cmake/Modules/FindVorbis.cmake\")\n")
    list(INSERT CMAKE_MODULE_PATH 0 "${PROXY_MODULE_DIR}")

    FetchContent_MakeAvailable(SFML)
endif()

# Common compiler warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Include directories for the entire project
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Define source files
set(SOURCES
        src/main.cpp
        src/core/engine.cpp
        src/core/rng.cpp
        src/core/payout.cpp
        src/ui/game_window.cpp
        src/ui/button.cpp
        src/ui/reel_view.cpp
        src/audio/sound_manager.cpp
        src/config/config_manager.cpp
        src/config/save_manager.cpp
)

# Define header files
set(HEADERS
        include/core/engine.h
        include/core/rng.h
        include/core/payout.h
        include/ui/game_window.h
        include/ui/button.h
        include/ui/reel_view.h
        include/audio/sound_manager.h
        include/config/config_manager.h
        include/config/save_manager.h
        include/config/save_data.h
)

# Main executable
add_executable(SlotMachine ${SOURCES} ${HEADERS})

if(ENABLE_DEBUG_LOGS)
    target_compile_definitions(SlotMachine PRIVATE ENABLE_DEBUG_LOGS=1)
else()
    target_compile_definitions(SlotMachine PRIVATE ENABLE_DEBUG_LOGS=0)
endif()

# Link libraries
target_link_libraries(SlotMachine
        PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
        nlohmann_json::nlohmann_json
)

# Include path
target_include_directories(SlotMachine
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Copy assets to build directory
add_custom_command(TARGET SlotMachine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:SlotMachine>/assets
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(BUILD_TESTS)
    enable_testing()

    add_executable(SlotMachineTests
            tests/test_main.cpp
            tests/test_save_manager.cpp
            tests/test_payout.cpp
            src/config/save_manager.cpp
            src/config/config_manager.cpp
            src/core/payout.cpp
            src/core/rng.cpp
    )

    target_include_directories(SlotMachineTests
            PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

    target_link_libraries(SlotMachineTests
            PRIVATE
            nlohmann_json::nlohmann_json
    )

    add_test(NAME SlotMachineTests COMMAND SlotMachineTests)

    set_tests_properties(SlotMachineTests PROPERTIES
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()

if(BUILD_DOCS)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        message(STATUS "Doxygen found.")
    endif()
endif()

install(TARGETS SlotMachine
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)
install(DIRECTORY assets/ DESTINATION bin/assets)

message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")