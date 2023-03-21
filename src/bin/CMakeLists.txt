add_executable(buffered_streams buffered_streams.c)
target_link_libraries(buffered_streams)

add_executable(environ get_environment.c)
target_link_libraries(environ)

add_executable(fork_ex fork_ex.c)
target_link_libraries(fork_ex)

add_executable(get_opts get_opts.c)
target_link_libraries(get_opts)
