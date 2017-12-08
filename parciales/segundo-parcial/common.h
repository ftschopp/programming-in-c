#ifndef COMMON_H
#define COMMON_H

#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Generate a random number
void rnd(long *prandom);
void rnd_between(long *prandom, long min, long max);
void random_init();
int random_generate(int min, int max);
//Generate key
key_t creo_clave(int id);

int* randomSinRepetir(int desde, int hasta, int cantidad);

//Generate segment memory
// int generateSegmentMemory(long size);
#endif
