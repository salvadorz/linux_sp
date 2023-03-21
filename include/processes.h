/*
 * @processes.c
 *
 * @version: 1.0
 * @Author:  Salvador Z
 * @brief:   processes
 */

#ifndef processes_H_
#define processes_H_

/**
 * @brief Creates a new program. arg_list must be a null termanted list
 * 
 * @param program to be executed from the path
 * @param arg_list 
 * @return int the proced ID of the child created
 */
int process_spawn(char *program, char **arg_list);

#endif // processes_H_