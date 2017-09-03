#include "common.h"

//Genera un numero aleatorio entre 0 y 65536
void rnd(long *prandom)
{
	*prandom = (25173 * *prandom + 13849) % 65536;
}