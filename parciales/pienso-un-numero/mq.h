#ifndef _MQ_H
#define _MQ_H

#include "base.h"

int createMessageQueue(int key);
int sendMessage(int id_queue, long mtype, MessageInfo info);
int receiveMessage(int id_queue, long mtype, MessageInfo* info);
void cleanQueue(int id_queue);

#endif