/***********************************************************************************
 * @file        mmcopier.c                                                         *
 * @author      Matt Ricci                                                         *
 * @brief       Multithreaded single-file copying utility.                         *
 ***********************************************************************************/

#include "mmcopier.h"

/* =============================================================================== */
/**
 * @brief Parses and validates command-line arguments for the `mmcopier` function.
 *
 * @param argc  Argument count.
 * @param argv  Argument vector, containing command-line arguments.
 * @param *n    Pointer to an integer where the parsed value of `n` will be stored.
 * @return      0 if parsing is successful, 1 if an error occurs.
 **
 * =============================================================================== */
int mmcopier_parse(int argc, char *argv[], int *n) {
  if (argc != ARGC) {
    printf("Argument Error: %s expected %d inputs, received %d.\n", argv[0], ARGC, argc);
    return 1;
  }
  if (sscanf(argv[1], "%d", n) != 1) {
    printf("Arugment Error: %s expects first input parameter to be integer type.\n", argv[0]);
    return 1;
  }
  if (*n > SIZE_N || *n < 1) {
    printf("Arugment Error: First parameter n must be greater than 1 and less than or equal to %d.\n", SIZE_N);
    return 1;
  }
  return 0;
}

void *mmcopier_copy() {
  printf("foo");
  pthread_exit(0);
}

int main(int argc, char *argv[]) {

  int n, ret;
  if ((ret = mmcopier_parse(argc, argv, &n)))
    return ret;

  for (int i = 0; i < n; i++) {
    pthread_t tid;
    pthread_attr_t attr;
    // @todo read in files from parsed directory argument
    // @todo pass in unique files to be copied into spawned thread
    pthread_attr_init(&attr);                         // Initialise attributes
    pthread_create(&tid, &attr, mmcopier_copy, NULL); // Spawn thread for file copy
    pthread_join(tid, NULL);                          // Join main thread to child
  }

  return 0;
}
