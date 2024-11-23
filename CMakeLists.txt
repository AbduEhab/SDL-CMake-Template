cmake_minimum_required(VERSION 3.16)

project(SDL_Template VERSION 0.1.0 LANGUAGES CXX)
enable_language(CXX)

# ----------------------------------------------------------------------------------------------
# Part till line 29 is very very similar to that of @codetechandtutorials

# DOWNLOAD ALL THE SUBMODULES
find_package(Git QUIET)

if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    # Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)

    if(GIT_SUBMODULE) # Check submodules during build (SET TO 0 TO AVOID UPDATING)
        message(STATUS "Submodule update")

        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive -f
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            RESULT_VARIABLE GIT_SUBMOD_RESULT)

        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

# ----------------------------------------------------------------------------------------------
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING
        "Build not specified, defaulted to RelWithDebInfo" FORCE)
    message(STATUS "Build type not specified, defaulted to RelWithDebInfo")
endif(NOT CMAKE_BUILD_TYPE)

# <-- SDL OPTIONS -->
set(USE_SDL2 ON)
set(USE_SDL_IMAGE OFF)
set(USE_SDL_MIXER OFF)
set(USE_SDL_TTF OFF)
set(USE_SDL_NET OFF)

# Add locations of other CMakelists.txt (one per call)
add_subdirectory(
    src
)

# Extra C++ specific option I need
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# <-- Set standard here -->
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

# <----------------------->

# <-- Define Compile Time Definitions here: -->
target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:DEBUG=1>
    $<$<CONFIG:RelWithDebInfo>:DEBUG=1>
    $<$<CONFIG:Release>:NDEBUG=1>
    $<$<CONFIG:MinSizeRel>:NDEBUG=1>
)

# <------------------------------------------->
if(USE_SDL2)
    # if folder not avalable
    if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/SDL)
        # Clone the submodule
        execute_process(
            COMMAND git submodule add -b SDL2 -f https://github.com/libsdl-org/SDL.git external/SDL
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        )
    endif()

    # Add the submodule directory to the include path
    include_directories(${PROJECT_SOURCE_DIR}/external)

    # Add the submodule library to the project
    add_subdirectory(${PROJECT_SOURCE_DIR}/external/SDL)

    # Link the submodule library to the main project
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2)

    # get ImGui
    if(NOT EXISTS ${PROJECT_SOURCE_DIR}external/imgui)
        execute_process(
            COMMAND git submodule add -f https://github.com/ocornut/imgui.git external/imgui
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        )
    endif()

    add_library(ImGui STATIC
        ${PROJECT_SOURCE_DIR}/external/imgui/imgui.h
        ${PROJECT_SOURCE_DIR}/external/imgui/imconfig.h
        ${PROJECT_SOURCE_DIR}/external/imgui/imgui_internal.h
        ${PROJECT_SOURCE_DIR}/external/imgui/imstb_rectpack.h
        ${PROJECT_SOURCE_DIR}/external/imgui/imstb_textedit.h
        ${PROJECT_SOURCE_DIR}/external/imgui/imstb_truetype.h
        ${PROJECT_SOURCE_DIR}/external/imgui/imgui.cpp
        ${PROJECT_SOURCE_DIR}/external/imgui/imgui_draw.cpp
        ${PROJECT_SOURCE_DIR}/external/imgui/imgui_widgets.cpp
        ${PROJECT_SOURCE_DIR}/external/imgui/imgui_tables.cpp
        ${PROJECT_SOURCE_DIR}/external/imgui/imgui_demo.cpp
        ${PROJECT_SOURCE_DIR}/external/imgui/backends/imgui_impl_sdl2.h
        ${PROJECT_SOURCE_DIR}/external/imgui/backends/imgui_impl_sdlrenderer2.h
        ${PROJECT_SOURCE_DIR}/external/imgui/backends/imgui_impl_sdl2.cpp
        ${PROJECT_SOURCE_DIR}/external/imgui/backends/imgui_impl_sdlrenderer2.cpp
    )

    target_include_directories(ImGui
        PRIVATE ${PROJECT_SOURCE_DIR}/external/imgui
        PRIVATE ${PROJECT_SOURCE_DIR}/external/SDL
    )

    target_link_libraries(ImGui
        PRIVATE SDL2
    )

    # Link the ImGui library to the main project
    target_link_libraries(${PROJECT_NAME} PRIVATE ImGui)

    # Add the ImGui include directory to the project
    target_include_directories(${PROJECT_NAME}
        PRIVATE ${PROJECT_SOURCE_DIR}/external/imgui)

else()
    message(STATUS "SDL3 is not supported yet but is coming soon (TM).")
endif()

if(USE_SDL_IMAGE)
    if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/SDL_image)
        execute_process(
            COMMAND git submodule add -b SDL2 -f https://github.com/libsdl-org/SDL_image.git external/SDL_image
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        )
    endif()

    set(SDL2IMAGE_INSTALL OFF)

    include_directories(${PROJECT_SOURCE_DIR}/external/SDL_image)

    add_subdirectory(${PROJECT_SOURCE_DIR}/external/SDL_image)

    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_image)
endif()

if(USE_SDL_MIXER)
    if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/SDL_mixer)
        execute_process(
            COMMAND git submodule add -b SDL2 -f https://github.com/libsdl-org/SDL_mixer.git external/SDL_mixer
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        )
    endif()

    set(SDL2MIXER_INSTALL OFF)

    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,--exclude-libs,SDL2_mixer.lib")

    # Add the submodule directory to the include path
    include_directories(${PROJECT_SOURCE_DIR}/external/SDL_mixer/include)

    # Add the submodule library to the project
    add_subdirectory(${PROJECT_SOURCE_DIR}/external/SDL_mixer)

    # Link the submodule library to the main project
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_mixer)
endif()

if(USE_SDL_TTF)
    if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/SDL_ttf)
        execute_process(
            COMMAND git submodule add -b SDL2 -f https://github.com/libsdl-org/SDL_ttf.git external/SDL_ttf
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        )
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external/
            RESULT_VARIABLE GIT_SUBMOD_RESULT)
    endif()

    set(SDL2TTF_INSTALL OFF)

    # Add the submodule directory to the include path
    include_directories(${PROJECT_SOURCE_DIR}/external/SDL_ttf)

    # Add the submodule library to the project
    add_subdirectory(${PROJECT_SOURCE_DIR}/external/SDL_ttf)

    # Link the submodule library to the main project
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_ttf)
endif()

if(USE_SDL_NET)
    if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/SDL_net)
        execute_process(
            COMMAND git submodule add -b SDL2 -f https://github.com/libsdl-org/SDL_net.git external/SDL_net
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        )
    endif()

    set(SDL2NET_INSTALL OFF)

    # Add the submodule directory to the include path
    include_directories(${PROJECT_SOURCE_DIR}/external/SDL_net)

    # Add the submodule library to the project
    add_subdirectory(${PROJECT_SOURCE_DIR}/external/SDL_net)

    # Link the submodule library to the main project
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_net)
endif()

# <-- Add directories to include directory here: -->
target_include_directories(${PROJECT_NAME}
    PUBLIC src
    PUBLIC include
)

if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/glm)
    execute_process(
        COMMAND git submodule add -f https://github.com/g-truc/glm.git external/glm
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    )
endif()

# <------------------------------------------------>
add_subdirectory(
    external/glm
)

# <-- Add directory of external libs to link here: -->
target_link_directories(${PROJECT_NAME}
    PRIVATE src # this line is just to prevent errors
)

# <-------------------------------------------------->

# <-- Link needed Libs: -->
target_link_libraries(${PROJECT_NAME}
    PUBLIC glm
)

# <----------------------->