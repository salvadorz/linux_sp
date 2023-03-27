# Example-1 Beffered strams vs Non-buffered
add_executable(buffered_streams buffered_streams.c)
target_link_libraries(buffered_streams)

# Example-2 to use vargs with get_opt frol Linux
add_executable(get_opts get_opts.c)
target_link_libraries(get_opts)

# Example-3 get Env vars
add_executable(environ get_environment.c)
target_link_libraries(environ)

# Example-4 Show the fork process
add_executable(fork_ex fork_ex.c)
target_link_libraries(fork_ex)

# Example-5 Fork and Exec a process and waitpid using static lib
add_executable(spawn_process spawn_process.c)
target_link_libraries(spawn_process process_execvp)

# Example-6 Sigaction for SIGHLD to detect process terminaion
add_executable(signal_disposition signal_disposition.c)
target_link_libraries(signal_disposition)

# Example-7 Sigaction for SIGHLD to detect process terminaion
add_executable(daemon_create daemon.c)
target_link_libraries(daemon_create)