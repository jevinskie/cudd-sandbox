set(CUDDSBX_CLI_SRC
    cli.cpp
)

set(CUDDSBX_CLI_PRIVATE_LIBS
    cudd-sandbox
    flags
    fmt::fmt
)

# CUDDSBX_CLI_HDR added for IDE project generation
add_executable(cudd-sandbox-cli ${CUDDSBX_CLI_SRC})

target_compile_options(cudd-sandbox-cli PRIVATE ${CUDDSBX_WARNING_FLAGS})
target_link_libraries(cudd-sandbox-cli
PRIVATE
    ${CUDDSBX_CLI_PRIVATE_LIBS}
)

set_target_properties(cudd-sandbox-cli PROPERTIES
    CXX_STANDARD 23
    CXX_EXTENSIONS ON
    CXX_STANDARD_REQUIRED ON
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
    OUTPUT_NAME cudd-sandbox
)

install(TARGETS cudd-sandbox-cli)
