#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include "mq.h"

int main(void)
{
    int msqid;

    msqid = createMessageQueue(4312);
    
    printf("spock: ready to receive messages, captain.\n");

    for(;;) 
    {
        MessageQueue mq;
        recvMessageQueue(msqid, &mq);
        printf("nro jugador %d nro pensado %d spock: \"%s\"\n",mq.info.nro_jugador, mq.info.nro_pensado, mq.info.mtext);
    }

    return 0;
}