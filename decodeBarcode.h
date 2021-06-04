#ifndef decodeBarcode_H_
#define decodeBarcode_H_

#include "validation.h"

char * readScannerSignal(char * fileName);
char * decodeRawSignal(char * rawSignal);
int decodeSignal(char * signal);
void reverseSignal(char *string);
static int roundNo(float num);
static int printResult(int numSymbols, int weight[]);

#endif