#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

typedef struct
{
    int nro_jugador;
    int nro_pensado;
    char mtext[200];
    
}MessageInfo;

typedef struct
{
    long mtype;
    MessageInfo info;
}MessageQueue;

struct my_msgbuf {
    long mtype;
    char mtext[200];
};

int main(void)
{
    //struct my_msgbuf buf;
    MessageQueue buf;
    int msqid;
    key_t key;

    if ((key = ftok("kirk.c", 'B')) == -1) {
        perror("ftok");
        exit(1);
    }

    if ((msqid = msgget(key, 0644 | IPC_CREAT)) == -1) {
        perror("msgget");
        exit(1);
    }
    
    printf("Enter lines of text, ^D to quit:\n");

    buf.mtype = 1; /* we don't really care in this case */

    while(fgets(buf.info.mtext, sizeof buf.info.mtext, stdin) != NULL) {
        int len = strlen(buf.info.mtext);

        /* ditch newline at end, if it exists */
        if (buf.info.mtext[len-1] == '\n') buf.info.mtext[len-1] = '\0';

        if (msgsnd(msqid, &buf, len+1, 0) == -1) /* +1 for '\0' */
            perror("msgsnd");
    }

    if (msgctl(msqid, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(1);
    }

    return 0;
}