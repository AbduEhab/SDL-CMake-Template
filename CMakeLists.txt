cmake_minimum_required(VERSION 3.13)

project(SDL_TEMPLATE VERSION 0.1.0 LANGUAGES CXX)
enable_language(CXX)

# ----------------------------------------------------------------------------------------------
# Part till line 71 is taken from @lefticus

# set a default CXX standard used by the external tools like clang-tidy, cppcheck, etc.
# You can later set fine-grained standards for each target using `target_compile_features`
set(CMAKE_CXX_STANDARD 20)

# ## Add project_options
# include(FetchContent)
# FetchContent_Declare(_project_options URL https://github.com/cpp-best-practices/project_options/archive/refs/heads/main.zip)
# FetchContent_MakeAvailable(_project_options)
# include(${_project_options_SOURCE_DIR}/Index.cmake)
include(CMakeSetupFiles/Index.cmake)

# run_vcpkg()

# Initialize project_options
# uncomment the options to enable them
if(MSVC)
    project_options(

        # ENABLE_CACHE
        # ENABLE_CONAN
        # WARNINGS_AS_ERRORS
        # ENABLE_CPPCHECK
        # ENABLE_CLANG_TIDY
        # ENABLE_INCLUDE_WHAT_YOU_USE
        # ENABLE_COVERAGE
        # ENABLE_PCH
        # PCH_HEADERS <Eigen/Dense> <fmt/core.h> <vector> <utility> <string> <string_view>
        # ENABLE_DOXYGEN
        # ENABLE_IPO
        # ENABLE_USER_LINKER
        # ENABLE_BUILD_WITH_TIME_TRACE
        # ENABLE_UNITY
        # ENABLE_SANITIZER_ADDRESS
        # ENABLE_SANITIZER_LEAK
        # ENABLE_SANITIZER_UNDEFINED_BEHAVIOR
        # ENABLE_SANITIZER_THREAD
        # ENABLE_SANITIZER_MEMORY
        CLANG_WARNINGS "-Weverything"
    )
else()
    project_options(

        # ENABLE_CACHE
        # ENABLE_CONAN
        # WARNINGS_AS_ERRORS
        # ENABLE_CPPCHECK
        # ENABLE_CLANG_TIDY
        # ENABLE_INCLUDE_WHAT_YOU_USE
        # ENABLE_COVERAGE
        # ENABLE_PCH
        # PCH_HEADERS <Eigen/Dense> <fmt/core.h> <vector> <utility> <string> <string_view>
        # ENABLE_DOXYGEN
        # ENABLE_IPO
        # ENABLE_USER_LINKER
        # ENABLE_BUILD_WITH_TIME_TRACE
        # ENABLE_UNITY
        # ENABLE_SANITIZER_ADDRESS
        # ENABLE_SANITIZER_LEAK
        # ENABLE_SANITIZER_UNDEFINED_BEHAVIOR
        # ENABLE_SANITIZER_THREAD
        # ENABLE_SANITIZER_MEMORY
    )
endif()

# ----------------------------------------------------------------------------------------------

# ----------------------------------------------------------------------------------------------
# Part till line 102 is very very similar to that of @codetechandtutorials

# DOWNLOAD ALL THE SUBMODULES
find_package(Git QUIET)

if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    # Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)

    if(NOT GIT_SUBMODULE) # Check submodules during build (SET TO 0 TO AVOID UPDATING)
        message(STATUS "Submodule update")

        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
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
	if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/external/SDL)
  	  # Clone the submodule
  	  execute_process(
  	      COMMAND git submodule add https://github.com/libsdl-org/SDL.git external/SDL
 	       WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
 	   )
 	   execute_process(
 	       COMMAND git checkout release-2.26.x
 	       WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/SDL/
 	   )
	endif()

	# Add the submodule directory to the include path
	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/include)

	# Add the submodule library to the project
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/SDL)

	# Link the submodule library to the main project
	target_link_libraries(${PROJECT_NAME} PRIVATE SDL2)
else()
	message(STATUS "SDL3 is not supported yet but is coming soon (TM).")
endif()

if(USE_SDL_IMAGE)
    if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/external/SDL_image)
        execute_process(
            COMMAND git submodule add https://github.com/libsdl-org/SDL_image.git external/SDL_image
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
        execute_process(
            COMMAND git checkout release-2.6.x
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/SDL_image/
        )
    endif()

    set(SDL2IMAGE_INSTALL OFF)

    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/SDL_image)

    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/SDL_image)

    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_image)
endif()

if(USE_SDL_MIXER)
    if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/external/SDL_mixer)
        execute_process(
            COMMAND git submodule add https://github.com/libsdl-org/SDL_mixer.git external/SDL_mixer --recursive
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
        execute_process(
            COMMAND git checkout release-2.6.x
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/SDL_mixer/
        )
    endif()

    set(SDL2MIXER_INSTALL OFF)

    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,--exclude-libs,SDL2_mixer.lib")

    # Add the submodule directory to the include path
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/SDL_mixer/include)

    # Add the submodule library to the project
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/SDL_mixer)

    # Link the submodule library to the main project
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_mixer)
endif()

if(USE_SDL_TTF)
    if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/external/SDL_ttf)
        execute_process(
            COMMAND git submodule add https://github.com/libsdl-org/SDL_ttf.git external/SDL_ttf --recursive
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
        execute_process(
            COMMAND git checkout release-2.20.x
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/SDL_ttf/
        )
    endif()

    set(SDL2TTF_INSTALL OFF)

    # Add the submodule directory to the include path
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/SDL_ttf)

    # Add the submodule library to the project
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/SDL_ttf)

    # Link the submodule library to the main project
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_ttf)
endif()

if(USE_SDL_NET)
    if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/external/SDL_net)
        execute_process(
            COMMAND git submodule add https://github.com/libsdl-org/SDL_net.git external/SDL_net
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
        execute_process(
            COMMAND git checkout release-2.2.x
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/SDL_net/
        )
    endif()

    set(SDL2NET_INSTALL OFF)

    # Add the submodule directory to the include path
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/SDL_net)

    # Add the submodule library to the project
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/SDL_net)

    # Link the submodule library to the main project
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_net)
endif()

# <-- Add directories to include directory here: -->
target_include_directories(${PROJECT_NAME}
    PUBLIC src
    PUBLIC include
    PUBLIC external/SDL/include
    PUBLIC external/glm
    PUBLIC external/sdl_image
)

# <------------------------------------------------>

# <-- Add directory of external libs to link here: -->
target_link_directories(${PROJECT_NAME}
    PRIVATE src # this line is just to prevent errors
)

# <-------------------------------------------------->

# <-- Link needed Libs: -->
target_link_libraries(${PROJECT_NAME}
    PRIVATE project_options project_warnings
)

# <----------------------->