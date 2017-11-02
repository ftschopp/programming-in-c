#include <stdio.h>
#include "mq.h"

int main(int argc, char *argv[])
{
    int mqid = createMessageQueue(4532);
    MessageInfo info;

     info.nro_jugador = 32;
     info.nro_pensado = 3;
     info.tipo = MSG_PIENSO;
     printf("Id queue %d", mqid);
   
    int res = sendMessage(mqid, 99, info);

    printf("Res %d",res);
    return 0;
}