# Example-1 Buffered strams vs Non-buffered
add_executable(01_buffered_streams 01_buffered_streams.c)

# Example-2 to use vargs with get_opt frol Linux
add_executable(02_get_opts 02_get_opts.c)

# Example-3 get Env vars
add_executable(03_environ 03_get_environment.c)

# Example-4 Show the fork process
add_executable(04_fork_ex 04_fork_ex.c)

# Example-5 Fork and Exec a process and waitpid using static lib
add_executable(05_spawn_process 05_spawn_process.c)
target_link_libraries(05_spawn_process process_execvp)

# Example-6 Sigaction for SIGHLD to detect process terminaion
add_executable(06_signal_disposition 06_signal_disposition.c)

# Example-7 Sigaction for SIGHLD to detect process terminaion
add_executable(07_daemon_create 07_daemon.c)

# Example-8 pThread Create
add_executable(08_thread_create 08_thread_create.c)
target_link_libraries(08_thread_create pthread)

# Example-9 pThread Banking
add_executable(09_threads_bank_withdrawals 09_threads_bank_withdrawals.c)
target_link_libraries(09_threads_bank_withdrawals banking pthread)

add_executable(10_sleep_app 10_sleep_app.c)
target_link_libraries(10_sleep_app sleep_types)
