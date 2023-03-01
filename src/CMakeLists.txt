cmake_minimum_required(VERSION 3.0.0)

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

# Copy Binary to DEV specified location
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
    ${CMAKE_SOURCE_DIR}/bin/${CMAKE_GENERATOR_PLATFORM} # Change this to your Prefered location (I prefer ${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/ or ${CMAKE_SOURCE_DIR}/bin/$<CMAKE_GENERATOR_PLATFORM>/)
)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    ${CMAKE_SOURCE_DIR}/bin/assets
)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/../assets
)

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
            ${CMAKE_BINARY_DIR}/external/SDL/$<CONFIG>/SDL2.dll
            ${CMAKE_SOURCE_DIR}/bin/${CMAKE_GENERATOR_PLATFORM}/SDL2.dll)

        add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_BINARY_DIR}/external/SDL/$<CONFIG>/SDL2.dll
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/SDL2.dll)

        if(USE_SDL_IMAGE)
            add_custom_command(
                TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_BINARY_DIR}/external/sdl_image/$<CONFIG>/SDL2_image.dll
                ${CMAKE_SOURCE_DIR}/bin/${CMAKE_GENERATOR_PLATFORM}/SDL2_image.dll)

            add_custom_command(
                TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_BINARY_DIR}/external/sdl_image/$<CONFIG>/SDL2_image.dll
                $<TARGET_FILE_DIR:${PROJECT_NAME}>/SDL2_image.dll)
        endif()

        if(USE_SDL_MIXER)
            add_custom_command(
                TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_BINARY_DIR}/external/SDL_mixer/$<CONFIG>/SDL2_mixer.dll
                ${CMAKE_SOURCE_DIR}/bin/${CMAKE_GENERATOR_PLATFORM}/SDL2_mixer.dll)

            add_custom_command(
                TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_BINARY_DIR}/external/SDL_mixer/$<CONFIG>/SDL2_mixer.dll
                $<TARGET_FILE_DIR:${PROJECT_NAME}>/SDL2_mixer.dll)
        endif()

        if(USE_SDL_TTF)
            add_custom_command(
                TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_BINARY_DIR}/external/SDL_ttf/$<CONFIG>/SDL2_ttf.dll
                ${CMAKE_SOURCE_DIR}/bin/${CMAKE_GENERATOR_PLATFORM}/SDL2_ttf.dll)

            add_custom_command(
                TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_BINARY_DIR}/external/SDL_ttf/$<CONFIG>/SDL2_ttf.dll
                $<TARGET_FILE_DIR:${PROJECT_NAME}>/SDL2_ttf.dll)
        endif()

        if(USE_SDL_NET)
            add_custom_command(
                TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_BINARY_DIR}/external/sdl_net/$<CONFIG>/SDL2_net.dll
                ${CMAKE_SOURCE_DIR}/bin/${CMAKE_GENERATOR_PLATFORM}/SDL2_net.dll)

            add_custom_command(
                TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_BINARY_DIR}/external/sdl_net/$<CONFIG>/SDL2_net.dll
                $<TARGET_FILE_DIR:${PROJECT_NAME}>/SDL2_net.dll)
        endif()
    else()
        message(STATUS "SDL3 is not supported yet but is coming soon (TM).")
    endif()
endif()

# Link assets needed for the binary to run

# add_custom_command(
# TARGET ${PROJECT_NAME} POST_BUILD
# COMMAND ${CMAKE_COMMAND} -E create_symlink ${link_src} ${link_dst}
# DEPENDS ${link_dst}
# COMMENT "symbolic link ${link_src} => ${link_dst}"
# )
set(CMAKE_INCLUDE_CURRENT_DIR ON)