add_library(usermod_eigenmath INTERFACE)

target_sources(usermod_eigenmath INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/eigenmath.c
)

target_include_directories(usermod_eigenmath INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(usermod INTERFACE usermod_eigenmath)


