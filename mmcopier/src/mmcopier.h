/***********************************************************************************
 * @file        mmcopier.h                                                         *
 * @author      Matt Ricci                                                         *
 ***********************************************************************************/

#ifndef _MMCOPIER_H
#define _MMCOPIER_H

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
#define MAX_FILENAME 50

typedef struct {
  struct dirent **files;
  int n;
  int fileCount;
  char fileName[MAX_FILENAME];
  char srcDir[FILENAME_MAX];
  char destDir[FILENAME_MAX];
} mmcopier;

int mmcopier_init(mmcopier *, int, char *[]);
int mmcopier_parse(int, char *[], int *);
void *mmcopier_copy(void *param);

#endif
