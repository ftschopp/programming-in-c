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
    int run = 1;
    int msqid = createMessageQueue(4532);
    
    int nroPensado = random_generate(MIN_NUM, MAX_NUM);

    printf("Numero Pensado %d", nroPensado);
    MessageQueue mq;
    int ganador = 0;
    while(run)
    {
        printf("Espero recibir msg\n");
        recvMessageQueue(msqid, &mq);
        
        printf("recibo Msg\n");
        showMessageQueue(&mq);

        MessageQueue rta;
        rta.mtype = MSG_JUGADOR + mq.info.nro_jugador;
        rta.info.nro_jugador = mq.info.nro_jugador;
        
        if(mq.info.nro_pensado == nroPensado)
        {
            //acerto
            run = 0;
            ganador = mq.info.nro_jugador;
            rta.info.event = EVT_ACERTO;
        }
        else
        {
            //no acerto
            rta.info.event = EVT_NO_ACERTO;
        }
        printf("envio Msg\n");
        showMessageQueue(&rta);
        sendMessageQueue(msqid, &rta);
        
        printf("sleep\n");
        usleep(100 * 1000);
    }

    printf("Gano el jugador numero %d", ganador);

    return 0;
}

