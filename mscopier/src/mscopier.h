/***********************************************************************************
 * @file        mscopier.h                                                         *
 * @author      Matt Ricci                                                         *
 ***********************************************************************************/

#ifndef _mscopier_H
#define _mscopier_H

#define _XOPEN_SOURCE 700
#include "dirent.h"
#include "pathcat.h"
#include "pthread.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"

#define ARGC         4
#define SIZE_N       10
#define SIZE_BUFF    20
#define MAX_FILENAME 50

// Debug print macro
#define PRINT_DEBUG(thread, string) \
  printf("[%s]\tthread=%d buffer=%d %s\n", thread, copier->i, copier->length, string)

// Vararg debug print macro
#define PRINT_DEBUGV(thread, string, ...)                                  \
  printf("[%s]\tthread=%d buffer=%d ", thread, copier->i, copier->length); \
  printf(string, __VA_ARGS__);

typedef struct mscopier {
  int i;
  int n;
  int length;
  FILE *infile;
  FILE *outfile;
  char *buff[SIZE_BUFF];
  pthread_mutex_t readMutex;
  pthread_mutex_t writeMutex;
  pthread_cond_t bufferNotFull;
  pthread_cond_t bufferNotEmpty;
} mscopier;

int mscopier_init(mscopier *, int, char *[]);
int mscopier_parse(int, char *[], int *);
void *mscopier_read(void *param);
void *mscopier_write(void *param);

#endif
