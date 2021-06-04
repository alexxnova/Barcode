#ifndef validation_H_
#define validation_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define CHARSIZE 6
#define NUMCODE 12

extern const char signalCode[NUMCODE][CHARSIZE];

int checkNoise(char * rawSignal, int i);
int checkStartStop(char * signal);
int checkConsistency(char * signal);
int validateCode(int n,int * arr);
static int equationC(int n,int * arr);
static int equationK(int n,int * arr);

#endif