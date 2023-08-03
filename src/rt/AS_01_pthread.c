#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h> /* system logs*/

#define NUM_THREADS   1
#define CMD_BUFF_SIZE 1024

void *thread_function(void *arg) {
  (void)arg;

  syslog(LOG_INFO, "Hello from Thread");
  printf("Hello from Thread\n");
  pthread_exit(NULL);
}

int print_command(char* cmd) {
  char commandOutput[CMD_BUFF_SIZE];

  if (!cmd)
    return EXIT_FAILURE;

  FILE *commandStream = popen(cmd, "r");
  if (commandStream == NULL) {
    fprintf(stderr, "Error executing command\n");
    return EXIT_FAILURE;
  }

  // Leer la salida del comando y almacenarla en commandOutput
  if (fgets(commandOutput, sizeof(commandOutput), commandStream) == NULL) {
    fprintf(stderr, "Error reading command output\n");
    return EXIT_FAILURE;
  }

  // Eliminar el salto de línea final si está presente
  char *newline = strchr(commandOutput, '\n');
  if (newline != NULL) {
    *newline = '\0';
  }

  // Imprimir la salida almacenada en commandOutput
  printf("$%s %s\n",cmd, commandOutput);

  // Cerrar el flujo de datos
  pclose(commandStream);

  return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
  int rc = EXIT_SUCCESS;
  int i;
  (void)argc;
  (void)argv;

  /**
   * Setup syslog logging with process name using the LOG_USER facility.
   */
  // openlog(argv[0], (LOG_ODELAY | LOG_PERROR), LOG_USER);

  // system("uname -a");
  
  if (argc > 1) 
    rc = print_command(argv[1]);
  else
    rc = print_command("uname -a");

  pthread_t thread_ids[NUM_THREADS] = { 0 };
  for (i = 0; i < NUM_THREADS; i++) {

  if(0 != pthread_create(&thread_ids[i],      // pointer to thread descriptor
                   (void *)0,                // use default attributes
                   thread_function, // thread function entry point
                   (void *)0                  // parameters to pass in
    )) {
    syslog(LOG_ERR, "Error creating %d Thread",i+1);
    rc = EXIT_FAILURE;
    }
  }

  // syslog(LOG_INFO, "Hello from Main");

  for (i = 0; i < NUM_THREADS; i++)
    if (0 != pthread_join(thread_ids[i], NULL)){
        printf("error joining %d",i+1);
        rc = EXIT_FAILURE;
    }
  
  if (EXIT_SUCCESS == rc)
    printf("Assignment 01 complete\n");

  // closelog();

  return rc;
}