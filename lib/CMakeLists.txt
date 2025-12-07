set(LIBCUDDSBX_SRC
    reorder.cpp
)

set(LIBCUDDSBX_HDR_STANDALONE
    cudd-sandbox.hpp
)

set(LIBCUDDSBX_HDR_STANDALONE_PRIVATE
    # common-internal.hpp
)

set(LIBCUDDSBX_HDR)
foreach(HDR ${LIBCUDDSBX_HDR_STANDALONE})
    set(RELHDR "${CUDDSBX_MAIN_INCLUDE_DIR}/cudd-sandbox/${HDR}")
    if(NOT EXISTS ${RELHDR})
        set(RELHDR "${CMAKE_CURRENT_BINARY_DIR}/${HDR}")
    endif()
    if(NOT EXISTS ${RELHDR})
        message(STATUS "LIBCUDDSBX_HDR_STANDALONE missing: ${HDR} ${RELHDR}")
    endif()
    list(APPEND LIBCUDDSBX_HDR ${RELHDR})
endforeach()

foreach(HDR ${LIBCUDDSBX_HDR_STANDALONE_PRIVATE})
    set(RELHDR "${CMAKE_CURRENT_SOURCE_DIR}/${HDR}")
    if(NOT EXISTS ${RELHDR})
        message(STATUS "LIBCUDDSBX_HDR_STANDALONE_PRIVATE missing: ${HDR} ${RELHDR}")
    endif()
    list(APPEND LIBCUDDSBX_HDR ${RELHDR})
endforeach()

foreach(SRC ${LIBCUDDSBX_SRC})
    get_filename_component(HDR_NAME ${SRC} NAME_WLE)
    set(HDR "${CUDDSBX_MAIN_INCLUDE_DIR}/cudd-sandbox/${HDR_NAME}.hpp")
    if(EXISTS ${HDR})
        list(APPEND LIBCUDDSBX_HDR ${HDR})
    endif()
endforeach()

set(LIBCUDDSBX_PUBLIC_LIBS
    fmt::fmt
    cudd
    dddmp
)

set(LIBCUDDSBX_PRIVATE_LIBS
    # etc
)

# LIBCUDDSBX_HDR added for IDE project generation
add_library(cudd-sandbox OBJECT ${LIBCUDDSBX_SRC} ${LIBCUDDSBX_HDR})
set_target_properties(cudd-sandbox PROPERTIES PUBLIC_HEADER "${LIBCUDDSBX_HDR}")
target_compile_options(cudd-sandbox PRIVATE ${CUDDSBX_WARNING_FLAGS})
target_link_libraries(cudd-sandbox
PUBLIC
    ${LIBCUDDSBX_PUBLIC_LIBS}
PRIVATE
    ${LIBCUDDSBX_PRIVATE_LIBS}
)

target_include_directories(cudd-sandbox
    PUBLIC ${CUDDSBX_MAIN_INCLUDE_DIR} ${CUDDSBX_BINARY_INCLUDE_DIR}
)

set_target_properties(cudd-sandbox PROPERTIES
    CXX_STANDARD 23
    CXX_EXTENSIONS ON
    CXX_STANDARD_REQUIRED ON
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
)
