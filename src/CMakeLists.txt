cmake_minimum_required(VERSION 3.16.0)

file(GLOB_RECURSE SRC_CXX_FILES CONFIGURE_DEPENDS *.cpp)
file(GLOB_RECURSE SRC_C_FILES CONFIGURE_DEPENDS *.c)
file(GLOB_RECURSE INC_CXX_FILES CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/include*.cpp)
file(GLOB_RECURSE INC_C_FILES CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/include*.c)

# Add source files to be compiled here (should be done automatically but im making it clear if you want to add more files)
add_executable(${PROJECT_NAME}
    ${SRC_CXX_FILES}
    ${SRC_C_FILES}
    ${INC_CXX_FILES}
    ${INC_C_FILES}
)

if(CMAKE_GENERATOR_PLATFORM STREQUAL "")
    if(UNIX AND NOT APPLE)
        set(CMAKE_GENERATOR_PLATFORM "linux")
    elseif(WIN32)
        set(CMAKE_GENERATOR_PLATFORM "windows")
    elseif(APPLE)
        set(CMAKE_GENERATOR_PLATFORM "macos")
    endif()

    message(WARNING "CMAKE_GENERATOR_PLATFORM is not set. 
                    Tried to guess the platform based on the generator.
                    It was set to ${CMAKE_GENERATOR_PLATFORM}")
endif(CMAKE_GENERATOR_PLATFORM STREQUAL "")

# Copy Binary to DEV specified location
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
    ${CMAKE_SOURCE_DIR}/bin/${CMAKE_GENERATOR_PLATFORM} # Change this to your Prefered location (I prefer ${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/ or ${CMAKE_SOURCE_DIR}/bin/$<CMAKE_GENERATOR_PLATFORM>/)
)

file(CREATE_LINK ${CMAKE_SOURCE_DIR}/assets assets SYMBOLIC)
file(CREATE_LINK ${CMAKE_SOURCE_DIR}/assets ${CMAKE_SOURCE_DIR}/bin/assets SYMBOLIC)

if(UNIX AND NOT APPLE)
    # Delete Cmake files
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory
        ${CMAKE_SOURCE_DIR}/bin/linux/CMakeFiles
    )

    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove
        ${CMAKE_SOURCE_DIR}/bin/linux/Makefile
    )

    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove
        ${CMAKE_SOURCE_DIR}/bin/linux/cmake_install.cmake
    )

else()
    if(USE_SDL2)
        add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_BINARY_DIR}/external/SDL/$<CONFIG>/SDL2$<$<CONFIG:Debug>:d>.dll
            ${CMAKE_SOURCE_DIR}/bin/${CMAKE_GENERATOR_PLATFORM}/SDL2$<$<CONFIG:Debug>:d>.dll)

        add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_BINARY_DIR}/external/SDL/$<CONFIG>/SDL2$<$<CONFIG:Debug>:d>.dll
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/SDL2$<$<CONFIG:Debug>:d>.dll)

        if(USE_SDL_IMAGE)
            add_custom_command(
                TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_BINARY_DIR}/external/sdl_image/$<CONFIG>/SDL2_image$<$<CONFIG:Debug>:d>.dll
                ${CMAKE_SOURCE_DIR}/bin/${CMAKE_GENERATOR_PLATFORM}/SDL2_image$<$<CONFIG:Debug>:d>.dll)

            add_custom_command(
                TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_BINARY_DIR}/external/sdl_image/$<CONFIG>/SDL2_image$<$<CONFIG:Debug>:d>.dll
                $<TARGET_FILE_DIR:${PROJECT_NAME}>/SDL2_image$<$<CONFIG:Debug>:d>.dll)
        endif()

        if(USE_SDL_MIXER)
            add_custom_command(
                TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_BINARY_DIR}/external/SDL_mixer/$<CONFIG>/SDL2_mixer$<$<CONFIG:Debug>:d>.dll
                ${CMAKE_SOURCE_DIR}/bin/${CMAKE_GENERATOR_PLATFORM}/SDL2_mixer$<$<CONFIG:Debug>:d>.dll)

            add_custom_command(
                TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_BINARY_DIR}/external/SDL_mixer/$<CONFIG>/SDL2_mixer$<$<CONFIG:Debug>:d>.dll
                $<TARGET_FILE_DIR:${PROJECT_NAME}>/SDL2_mixer$<$<CONFIG:Debug>:d>.dll)
        endif()

        if(USE_SDL_TTF)
            add_custom_command(
                TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_BINARY_DIR}/external/SDL_ttf/$<CONFIG>/SDL2_ttf$<$<CONFIG:Debug>:d>.dll
                ${CMAKE_SOURCE_DIR}/bin/${CMAKE_GENERATOR_PLATFORM}/SDL2_ttf$<$<CONFIG:Debug>:d>.dll)

            add_custom_command(
                TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_BINARY_DIR}/external/SDL_ttf/$<CONFIG>/SDL2_ttf$<$<CONFIG:Debug>:d>.dll
                $<TARGET_FILE_DIR:${PROJECT_NAME}>/SDL2_ttf$<$<CONFIG:Debug>:d>.dll)
        endif()

        if(USE_SDL_NET)
            add_custom_command(
                TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_BINARY_DIR}/external/sdl_net/$<CONFIG>/SDL2_net$<$<CONFIG:Debug>:d>.dll
                ${CMAKE_SOURCE_DIR}/bin/${CMAKE_GENERATOR_PLATFORM}/SDL2_net$<$<CONFIG:Debug>:d>.dll)

            add_custom_command(
                TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_BINARY_DIR}/external/sdl_net/$<CONFIG>/SDL2_net$<$<CONFIG:Debug>:d>.dll
                $<TARGET_FILE_DIR:${PROJECT_NAME}>/SDL2_net$<$<CONFIG:Debug>:d>.dll)
        endif()
    else()
        message(STATUS "SDL3 is not supported yet but is coming soon (TM).")
    endif()
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)