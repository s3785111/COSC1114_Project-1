/***********************************************************************************
 * @file        mmcopier.c                                                         *
 * @author      Matt Ricci                                                         *
 * @brief       Multithreaded multi-file copying utility.                          *
 ***********************************************************************************/

#include "mmcopier.h"

/* =============================================================================== */
/**
 * @brief Initializes the mmcopier struct.
 *
 * @param *copier  Pointer to the mmcopier structure to be initialized.
 * @param argc     Argument count from the command line.
 * @param argv     Argument vector, containing command-line arguments.
 * @return         0 on success, 1 on error.
 **
 * =============================================================================== */

int mmcopier_init(mmcopier *copier, int argc, char *argv[]) {
  // Parse input parameters, early exit on error
  int n, ret;
  if ((ret = mmcopier_parse(argc, argv, &n)))
    return ret;

  // Retrieve string representation of working directory
  char workingDir[FILENAME_MAX];
  getcwd(workingDir, FILENAME_MAX);

  // ============================
  //       ERROR CHECKING
  // ============================

  pathcat(copier->srcDir, workingDir, argv[2]);  // Concatenate source directory path with validation
  pathcat(copier->destDir, workingDir, argv[3]); // Concatenate destination directory path with validation

  // Read files from source directory and ensure it exists
  struct dirent **files;
  if (scandir(copier->srcDir, &files, NULL, alphasort) < n) {
    printf("Path error: Insufficient files or directory %s does not exist\n", copier->srcDir);
    return 1;
  }

  // ============================
  //       INITIALISATION
  // ============================

  // Initialise struct and return
  copier->n     = n;
  copier->files = files;
  return 0;
}

/* =============================================================================== */
/**
 * @brief Parses and validates command-line arguments for the `mmcopier` function.
 *
 * @param argc  Argument count.
 * @param argv  Argument vector, containing command-line arguments.
 * @param *n    Pointer to an integer where the parsed value of `n` will be stored.
 * @return      0 on success, 1 on error.
 **
 * =============================================================================== */

int mmcopier_parse(int argc, char *argv[], int *n) {
  if (argc != ARGC) {
    printf("Argument Error: Expected %d inputs, received %d.\n", ARGC, argc);
    return 1;
  }
  if (sscanf(argv[1], "%d", n) != 1) {
    printf("Arugment Error: Expected first input parameter to be integer type.\n");
    return 1;
  }
  if (*n > SIZE_N || *n < 1) {
    printf("Arugment Error: First parameter n must be greater than 1 and less than or equal to %d.\n", SIZE_N);
    return 1;
  }
  return 0;
}

/* =============================================================================== */
/**
 * @brief
 *
 * @return
 **
 * =============================================================================== */

void *mmcopier_copy(void *param) {
  mmcopier copier = *(mmcopier *)param;

  // Build source and destination filenames
  char src[FILENAME_MAX], dest[FILENAME_MAX];
  pathcat(src, copier.srcDir, copier.fileName);
  pathcat(dest, copier.destDir, copier.fileName);

  // Open files from struct
  FILE *infile  = fopen(src, "r");
  FILE *outfile = fopen(dest, "w+");

#ifdef DEBUG
  printf("Copying %s to %s\n", src, dest);
#endif

  // Copy each character from source to destination file
  int a;
  while ((a = fgetc(infile)) != EOF) {
    fputc(a, outfile);
  }

  pthread_exit(0);
}

/* =============================================================================== */
/**
 * @brief Main function. Initialises the copier and spawns threads to copy files.
 **
 * =============================================================================== */

int main(int argc, char *argv[]) {

  int ret;
  mmcopier copier;

  // Initialise copier
  if ((ret = mmcopier_init(&copier, argc, argv)))
    return ret;

  // Loop through files ignoring . and ..
  for (int i = 2; i < 2 + copier.n; i++) {
    pthread_t tid;
    pthread_attr_t attr;

    // Spawn thread for file and execute copy
    pthread_attr_init(&attr);                            // Initialise attributes
    strcpy(copier.fileName, copier.files[i]->d_name);    // Copy input file name to copier struct
    pthread_create(&tid, &attr, mmcopier_copy, &copier); // Spawn thread for file copy and pass in copier
    pthread_join(tid, NULL);                             // Join main thread to child
  }

  return 0;
}
