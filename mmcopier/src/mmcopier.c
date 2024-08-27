/***********************************************************************************
 * @file        mmcopier.c                                                         *
 * @author      Matt Ricci                                                         *
 * @brief       Multithreaded multi-file copying utility.                          *
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

void *mmcopier_copy(void *param) {
  FILE *infile  = fopen((char *)param, "r");
  FILE *outfile = fopen((char *)param, "w+");
  pthread_exit(0);
}

int main(int argc, char *argv[]) {

  // Parse input parameters, early exit on error
  int n, ret;
  if ((ret = mmcopier_parse(argc, argv, &n)))
    return ret;

  // Retrieve string representation of working directory
  char workingDir[FILENAME_MAX];
  char srcDir[FILENAME_MAX];
  char destDir[FILENAME_MAX];
  getcwd(workingDir, FILENAME_MAX);

  // ============================
  //       ERROR CHECKING
  // ============================

  // Concatenate source directory path and check length of pathname
  size_t fileLength = snprintf(srcDir, FILENAME_MAX, "%s/%s", workingDir, argv[2]);
  if (fileLength > sizeof(srcDir)) {
    printf("Path Error: Source path too long\n");
    return 1;
  }

  // Concatenate destination directory path and check length of pathname
  fileLength = snprintf(destDir, FILENAME_MAX, "%s/%s", workingDir, argv[3]);
  if (fileLength > sizeof(destDir)) {
    printf("Path Error: Destination path too long\n");
    return 1;
  }

  // Read files from source directory and ensure it exists
  struct dirent **files;
  if (scandir(srcDir, &files, NULL, alphasort) < n) {
    printf("Path error: Insufficient files or directory %s does not exist\n", srcDir);
    return 1;
  }

  // ============================
  //       THREAD HANDLING
  // ============================

  // Copy source and destination directories to args struct
  mmcopier_args args;
  strcpy(args.destDir, destDir);
  strcpy(args.srcDir, srcDir);

  // Loop through files ignoring . and ..
  for (int i = 2; i < 2 + n; i++) {
    pthread_t tid;
    pthread_attr_t attr;

    // Spawn thread for file and execute copy
    pthread_attr_init(&attr);                          // Initialise attributes
    strcpy(args.fileName, files[i]->d_name);           // Copy input file name to args struct
    pthread_create(&tid, &attr, mmcopier_copy, &args); // Spawn thread for file copy and pass in filename
    pthread_join(tid, NULL);                           // Join main thread to child
  }

  return 0;
}
