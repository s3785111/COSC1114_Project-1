/***********************************************************************************
 * @file        mmcopier.h                                                         *
 * @author      Matt Ricci                                                         *
 * @brief                                                                          *
 ***********************************************************************************/

#ifndef _MMCOPIER_H
#define _MMCOPIER_H

#include "pthread.h"
#include "stdint.h"
#include "stdio.h"

#define ARGC   4
#define SIZE_N 10

int mmcopier_parse(int, char *[], int *);
void *mmcopier_copy();

#endif
