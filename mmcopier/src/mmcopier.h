/***********************************************************************************
 * @file        mmcopier.h                                                         *
 * @author      Matt Ricci                                                         *
 * @brief                                                                          *
 ***********************************************************************************/

#ifndef _MMCOPIER_H
#define _MMCOPIER_H

#define _XOPEN_SOURCE 700
#include "dirent.h"
#include "pthread.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "unistd.h"

#define ARGC   4
#define SIZE_N 10

typedef struct {
  char fileName[FILENAME_MAX];
  char srcDir[FILENAME_MAX];
  char destDir[FILENAME_MAX];
} mmcopier_args;

int mmcopier_parse(int, char *[], int *);
void *mmcopier_copy(void *param);

#endif
