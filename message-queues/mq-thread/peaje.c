#include "funciones.h"
#include "definiciones.h"
#include <pthread.h>

#define VEHICULOS_DESDE 1
#define VEHICULOS_HASTA 2
#define MAX_LIBERAR 10

typedef struct tipo_peaje tpeaje;
struct tipo_peaje
{
	int 	nro_via;						
	int  id_colamensaje;
	int  cantidad_liberaciones;
	
};

void *ThreadVia (void *parametro)
{
	int 			cantidad_vehiculos_en_cola=0;
	int 			cantidad_liberaciones=0;
	int 			nro_via;
	int 			id_cola_mensajes;
	int 			done=0;
	int 			vehiculos;
	mensaje		msg;
	
	tpeaje *datos_thread = (tpeaje*) parametro;
	nro_via = datos_thread->nro_via;
	id_cola_mensajes = datos_thread->id_colamensaje;
	
	
	printf("Soy la via %d\n", nro_via+1);

	
	while(done==0)
	{
		printf("\nVIA:%d DESTINO:%d\n", nro_via, MSG_VIAS+nro_via);
		recibir_mensaje(id_cola_mensajes, MSG_VIAS+nro_via, &msg);	//bloqueate
	
		/*printf("VIA:%d Destino   %d ", nro_via, (int) msg.long_dest);
		printf("Remitente %d ", msg.int_rte);
		printf("Evento    %d ", msg.int_evento);
		printf("Mensaje   %s ", msg.char_mensaje);*/
	
		switch (msg.int_evento)
		{
			case EVT_VEHICULOS:
				cantidad_vehiculos_en_cola += atoi(msg.char_mensaje);
				printf("\nVIA:%d Cantidad de Vehiculos %d\n", nro_via, cantidad_vehiculos_en_cola);
				vehiculos = rand()%(VEHICULOS_HASTA+1-VEHICULOS_DESDE)+VEHICULOS_DESDE;
				printf("\nVIA:%d Saco %d Vehiculos \n", nro_via, vehiculos);
				
				cantidad_vehiculos_en_cola -= vehiculos;
				if(cantidad_vehiculos_en_cola<0)
					cantidad_vehiculos_en_cola=0;
				printf("\nVIA:%d Cantidad de Vehiculos %d\n", nro_via, cantidad_vehiculos_en_cola);
				if(cantidad_vehiculos_en_cola>=MAX_LIBERAR)
				{
					cantidad_liberaciones++;
					cantidad_vehiculos_en_cola=0;
					printf("\nVIA:%d Libere %d veces \n", nro_via, cantidad_liberaciones);
				}				
			break;		    	
			case EVT_FINALIZAR:
				printf("\nVIA:%d Finalizar\n", nro_via);
				printf("VIA:%d Libere %d veces \n", nro_via, cantidad_liberaciones);
				datos_thread->cantidad_liberaciones = cantidad_liberaciones;
				done=1;
			break;		    	
			default:
				printf("\nVIA:%d Evento sin definir\n", nro_via);
			break;
		}
	};
	pthread_exit ((void *)"Listo");
}
		
int main(int argc, char *argv[])
{
	int id_cola_mensajes;
	int i, ctl = 0;
	int cantidad=1;
	
	tpeaje *datos_thread;
	
	srand(time(NULL));
		
	if (argc>1)
		cantidad = atoi(argv[1]);

	printf("%d\n", cantidad);
	
	id_cola_mensajes 	= creo_id_cola_mensajes(CLAVE_BASE);

	borrar_mensajes(id_cola_mensajes); //Borra todos los mensajes que haya en la cola.


	pthread_t* idHilo = (pthread_t* ) malloc(sizeof(pthread_t)*cantidad);
	pthread_attr_t 	atributos;
	pthread_attr_init (&atributos);
	pthread_attr_setdetachstate (&atributos, PTHREAD_CREATE_JOINABLE);
	
	datos_thread = (tpeaje*) malloc(sizeof(tpeaje)*cantidad);
	
	for(i=0; i<cantidad; i++)
	{
		datos_thread[i].nro_via = i;
		datos_thread[i].id_colamensaje = id_cola_mensajes;
		datos_thread[i].cantidad_liberaciones=0;
		
		pthread_create (&idHilo[i], &atributos, ThreadVia, &datos_thread[i]);
	}
	
	for(i=0; i<cantidad; i++)
	{
		pthread_join (idHilo[i], NULL);
		printf("PPAL: Recibi de la via %d: %d\n", i+1, datos_thread[i].cantidad_liberaciones);
		ctl += datos_thread[i].cantidad_liberaciones;
	}			
	printf("PPAL: TOTAL %d\n", ctl);
	return 0;
}



