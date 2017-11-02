#include <stdio.h>
#include "mq.h"

int main(int argc, char *argv[])
{
    int mqid = createMessageQueue(4532);
    MessageInfo info;

     
     printf("Id queue %d", mqid);
   
    int res = receiveMessage(mqid, 99, &info);

    printf("Res %d\n",res);
    printf("Datos %d %d %d \n", info.nro_jugador, info.nro_pensado, info.tipo);
    return 0;
}