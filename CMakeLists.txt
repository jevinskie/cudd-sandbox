cmake_minimum_required(VERSION 4.1)

project(clang-unformat-ng C CXX)

option(UNFMTNG_DISABLE_LLVM "Stub out LLVM functionality to make a build that is ammenable for -fsanitize." OFF)

option(FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." ON)

if (${FORCE_COLORED_OUTPUT})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
       set(JEV_COLOR_DIAGS -fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
       set(JEV_COLOR_DIAGS -fcolor-diagnostics)
    else()
        message(WARNING "COLOR_DIAGS: Compiler not detected: ${CMAKE_CXX_COMPILER_ID}")
        set(JEV_COLOR_DIAGS)
    endif ()
endif ()

set(UNFMTNG_MAIN_SRC_DIR       ${CMAKE_CURRENT_SOURCE_DIR}    )
set(UNFMTNG_MAIN_INCLUDE_DIR   ${UNFMTNG_MAIN_SRC_DIR}/include)
set(UNFMTNG_BINARY_DIR         ${CMAKE_CURRENT_BINARY_DIR}    )
set(UNFMTNG_BINARY_INCLUDE_DIR ${UNFMTNG_BINARY_DIR}/include  )

# needed on linux to build exe w/ object library
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(UNFMTNG_WARNING_FLAGS
    -Wall
    -Wextra
    -Wpedantic
    -Wno-unused-parameter
    -Wno-missing-field-initializers
    -Wno-gnu-statement-expression-from-macro-expansion
    -Werror=return-type
)

add_compile_options(${JEV_COLOR_DIAGS})

if (APPLE)
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
elseif (LINUX)
    # add_compile_options(-fsanitize=address -O0 -fno-omit-frame-pointer -mno-omit-leaf-frame-pointer)
    # add_link_options(-fsanitize=address -O0 -fno-omit-frame-pointer -mno-omit-leaf-frame-pointer)
    # add_compile_options(-fsanitize=memory -fsanitize-memory-track-origins -stdlib=libc++)
    # add_link_options(-fsanitize=memory -fsanitize-memory-track-origins -stdlib=libc++)
endif()
# add_compile_options(-fsanitize=address)
# add_link_options(-fsanitize=address)
# add_compile_options(-fsanitize=undefined)
# add_link_options(-fsanitize=undefined)
# add_compile_options(-fsanitize=memory)
# add_link_options(-fsanitize=memory)
# add_compile_options(-fsanitize=thread)
# add_link_options(-fsanitize=thread)
# add_compile_options(-fsanitize=type)
# add_link_options(-fsanitize=type)


add_subdirectory(3rdparty)
add_subdirectory(lib)
add_subdirectory(tools)
