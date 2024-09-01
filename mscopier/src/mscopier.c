/***********************************************************************************
 * @file        mscopier.c                                                         *
 * @author      Matt Ricci                                                         *
 * @brief       Multithreaded single-file copying utility.                         *
 ***********************************************************************************/

#include "mscopier.h"

/* =============================================================================== */
/**
 * @brief Initializes the mscopier struct.
 * @todo Move pathcat to mscopier_parse with added struct input to function
 *
 * @param *copier  Pointer to the mscopier structure to be initialized.
 * @param argc     Argument count from the command line.
 * @param argv     Argument vector, containing command-line arguments.
 * @return         0 on success, 1 on error.
 **
 * =============================================================================== */

int mscopier_init(mscopier *copier, int argc, char *argv[]) {
  // Parse input parameters, early exit on error
  int n, ret;
  if ((ret = mscopier_parse(argc, argv, &n)))
    return ret;

  // Initialise buffer
  for (int i = 0; i < SIZE_BUFF; i++)
    copier->buff[i] = NULL;

  // Retrieve string representation of working directory
  char workingDir[FILENAME_MAX];
  getcwd(workingDir, FILENAME_MAX);

  // Construct full paths for input and output files
  char infile[FILENAME_MAX], outfile[FILENAME_MAX];
  pathcat(infile, workingDir, argv[2]);  // Concatenate source directory path
  pathcat(outfile, workingDir, argv[3]); // Concatenate destination directory path

  // Initialise struct params
  copier->n       = n;
  copier->i       = 0;
  copier->length  = 0;
  copier->infile  = fopen(infile, "r");
  copier->outfile = fopen(outfile, "w+");

  // Validate input file
  if (copier->infile == NULL) {
    fprintf(stderr, "File Error: File %s not found.\n", infile);
    return 1;
  }

  // Initialise pthread objects
  pthread_mutex_init(&copier->readMutex, NULL);
  pthread_mutex_init(&copier->writeMutex, NULL);
  pthread_cond_init(&copier->bufferNotFull, NULL);
  pthread_cond_init(&copier->bufferNotEmpty, NULL);
  return 0;
}

/* =============================================================================== */
/**
 * @brief Parses and validates command-line arguments for the `mscopier` function.
 *
 * @param argc  Argument count.
 * @param argv  Argument vector, containing command-line arguments.
 * @param *n    Pointer to an integer where the parsed value of `n` will be stored.
 * @return      0 on success, 1 on error.
 **
 * =============================================================================== */

int mscopier_parse(int argc, char *argv[], int *n) {
  if (argc != ARGC) {
    fprintf(stderr, "Argument Error: Expected %d inputs, received %d.\n", ARGC, argc);
    return 1;
  }
  if (sscanf(argv[1], "%d", n) != 1) {
    fprintf(stderr, "Arugment Error: Expected first input parameter to be integer type.\n");
    return 1;
  }
  if (*n > SIZE_N || *n < 1) {
    fprintf(stderr, "Arugment Error: First parameter n must be greater than 1 and less than or equal to %d.\n", SIZE_N);
    return 1;
  }
  return 0;
}

/* =============================================================================== */
/**
 * @brief Reads a line from the input file into the copier's buffer.
 *
 * @param param Pointer to the mscopier structure.
 * @return None.
 **
 * =============================================================================== */

void *mscopier_read(void *param) {
  mscopier *copier = (mscopier *)param;
  size_t buffSize  = SIZE_BUFF;

  pthread_mutex_lock(&copier->readMutex);

#ifdef DEBUG
  PRINT_DEBUG("READ", "Waiting on bufferNotFull");
#endif

  // Wait until space is available in the buffer
  if (copier->length == SIZE_BUFF)
    pthread_cond_wait(&copier->bufferNotFull, &copier->readMutex);

#ifdef DEBUG
  PRINT_DEBUG("READ", "Reading line from file...");
#endif

  // Read line from file into shared access buffer
  if (getline(&copier->buff[copier->length], &buffSize, copier->infile) == -1) {
    // Unlock and exit thread on error or EOF
    pthread_mutex_unlock(&copier->readMutex);
    pthread_exit(NULL);
  }

#ifdef DEBUG
  PRINT_DEBUGV("READ", "Read string: %s", copier->buff[copier->length]);
#endif

  // Signal that buffer is not empty and increment length
  copier->length++;
  pthread_cond_signal(&copier->bufferNotEmpty);

  pthread_mutex_unlock(&copier->readMutex);
  pthread_exit(0);
}

/* =============================================================================== */
/**
 * @brief
 *
 * @return
 **
 * =============================================================================== */

void *mscopier_write(void *param) {
  mscopier *copier = (mscopier *)param;

  pthread_mutex_lock(&copier->writeMutex);

#ifdef DEBUG
  PRINT_DEBUG("WRITE", "Waiting on bufferNotEmpty");
#endif

  // Wait until a line is available in the buffer
  if (copier->length == 0)
    pthread_cond_wait(&copier->bufferNotEmpty, &copier->writeMutex);

#ifdef DEBUG
  PRINT_DEBUG("WRITE", "Writing line to file...");
#endif

  // Write line from shared access buffer to file
  fputs(copier->buff[copier->length - 1], copier->outfile);

#ifdef DEBUG
  PRINT_DEBUGV("WRITE", "Wrote string: %s", copier->buff[copier->length - 1]);
#endif

  // Free the allocated memory for the line
  free(copier->buff[copier->length - 1]);
  copier->buff[copier->length - 1] = NULL;

  // Signal that buffer is not full and decrement length
  copier->length--;
  pthread_cond_signal(&copier->bufferNotFull);

  pthread_mutex_unlock(&copier->writeMutex);
  pthread_exit(0);
}

/* =============================================================================== */
/**
 * @brief Main function. Initialises the copier and spawns threads to copy files.
 **
 * =============================================================================== */

int main(int argc, char *argv[]) {

  int ret;
  mscopier copier;

  // Initialise copier
  if ((ret = mscopier_init(&copier, argc, argv)))
    return ret;

#ifdef DEBUG
  printf("BEGINNING MAIN PROGRAM\n");
#endif

  // Spawn and enter n-count read/write pair threads
  for (copier.i = 0; copier.i < copier.n; copier.i++) {
    pthread_t tidRead, tidWrite;
    pthread_attr_t attr;

    // Spawn thread for file and execute copy
    pthread_attr_init(&attr);                                  // Initialise attributes
    pthread_create(&tidRead, &attr, mscopier_read, &copier);   // Spawn thread for file read
    pthread_create(&tidWrite, &attr, mscopier_write, &copier); // Spawn thread for file write

    // Join main thread to children
    pthread_join(tidRead, NULL);
    pthread_join(tidWrite, NULL);
  }

  // Destory mutex
  pthread_mutex_destroy(&copier.readMutex);
  pthread_mutex_destroy(&copier.writeMutex);

#ifdef DEBUG
  printf("EXITING MAIN PROGRAM\n");
#endif

  return 0;
}
