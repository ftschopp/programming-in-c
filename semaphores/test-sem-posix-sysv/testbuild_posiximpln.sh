cc -c semaphore_posix.c -o semaphore_posix.o
cc -bhalt:5 -bM:SRE -Wl,-brtl -bnoipath -G -o libsemaphore.a semaphore_posix.o

cc -c test1_posiximpln.c
cc -L . -o test1posiximpln -lpthread -lsemaphore test1_posiximpln.o

