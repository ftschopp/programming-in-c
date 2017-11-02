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
//#include "base.h"

int main(int argc, char *argv[])
{

     int mqid = createMessageQueue(4532);
    // MessageInfo info;

    // info.nro_jugador =32;
    // info.nro_pensado = 3;
    // //info.tipo = MSG_PIENSO;
    // printf("Id queue %d", mqid);


    return 0;
    
}