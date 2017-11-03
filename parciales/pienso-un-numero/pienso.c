#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <time.h>
#include <string.h>
#include "mq.h"
#include "common.h"

#define MIN_NUM 1
#define MAX_NUM 99

int main(int argc, char *argv[])
{
    random_init();
    
    int mqid = createMessageQueue(4532);
    int nroPensado = random_generate(MIN_NUM, MAX_NUM);

    

    // MessageInfo info;

    // info.nro_jugador =32;
    // info.nro_pensado = 3;
    // //info.tipo = MSG_PIENSO;
    // printf("Id queue %d", mqid);


    return 0;
    
}