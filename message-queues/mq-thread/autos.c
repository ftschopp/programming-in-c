#include "funciones.h"
#include "definiciones.h"

#define INTERVALO_DESDE 100
#define INTERVALO_HASTA 500

#define VEHICULOS_DESDE 1
#define VEHICULOS_HASTA 5



int main(int argc, char *argv[])
{
	int id_cola_mensajes;
	int i, j;
	int cantidad_vias=1;
	int intervalo=0;
	int total_vehiculos=0;
	int vehiculos=0;
	int nro_via=0;
	char cadena[100];
	
	if (argc>2)
	{
		cantidad_vias = atoi(argv[1]);
		total_vehiculos = atoi(argv[2]); //este sería por teclado.
	}
	printf("%d\n", cantidad_vias);
	
	id_cola_mensajes 	= creo_id_cola_mensajes(CLAVE_BASE);
	srand(time(NULL));
	
	while(total_vehiculos>0)
	{
		intervalo = rand()%(INTERVALO_HASTA+1-INTERVALO_DESDE)+INTERVALO_DESDE;
		vehiculos = rand()%(VEHICULOS_HASTA+1-VEHICULOS_DESDE)+VEHICULOS_DESDE;
		nro_via = rand()%(cantidad_vias-0)+0;
		
		printf("VIA:%d VEHICULOS:%d INTERVALO:%d\n", nro_via, vehiculos, intervalo);
		
		sprintf(cadena, "%d", vehiculos);
		enviar_mensaje(id_cola_mensajes , MSG_VIAS+nro_via, MSG_AUTOS, EVT_VEHICULOS, cadena);
		total_vehiculos-=vehiculos;
		printf("Quedan %d vehiculos\n", total_vehiculos);
		usleep(1000*intervalo);
	}

	for(i=0; i<cantidad_vias; i++)
	{
		enviar_mensaje(id_cola_mensajes , MSG_VIAS+i, MSG_AUTOS, EVT_FINALIZAR, "FINALIZAR");
	}
	
	return 0;
}

    
