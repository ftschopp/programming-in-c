#include "funciones.h"
#include "definiciones.h"
 
#define INTERVALO_PEDIDOS 5000

stock	inventario[CANTIDAD_TOTAL];

int	cargar_codigos(codigos *ptr_codigos)
{
	ptr_codigos[0].codigo = 1;
	strcpy(ptr_codigos[0].descripcion, "LECHE");
	
	ptr_codigos[1].codigo = 2;
	strcpy(ptr_codigos[1].descripcion, "HUEVOS");

	ptr_codigos[2].codigo = 3;
	strcpy(ptr_codigos[2].descripcion, "MANTECA");

	ptr_codigos[3].codigo = 4;
	strcpy(ptr_codigos[3].descripcion, "AZUCAR");
	return 0;
}		

void procesar_evento(int id_cola_mensajes, mensaje msg)
{
	char 	cadena[LARGO_TMENSAJE];
	int 	count = 0;
	int		codigo=0;
	int 	cantidad=0;
	char 	*token;
	int	i;

	printf("Destino   %d\n", (int) msg.long_dest);
	printf("Remitente %d\n", msg.int_rte);
	printf("Evento    %d\n", msg.int_evento);
	printf("Mensaje   %s\n", msg.char_mensaje);
	switch (msg.int_evento)
	{
		case EVT_CONSULTA_STOCK:
			printf("\nConsulta Stock\n");
			codigo = atoi(msg.char_mensaje); 
			printf("COD:%d CANT:%d\n", inventario[codigo-1].codigo, inventario[codigo-1].cantidad);
			sprintf(cadena, "%d|%d", inventario[codigo-1].codigo, inventario[codigo-1].cantidad);
			enviar_mensaje(id_cola_mensajes , msg.int_rte, MSG_SUPERMERCADO, EVT_RESPUESTA_STOCK, cadena);
		break;
		case EVT_SUMA_STOCK:
			printf("Suma Stock\n");
			count = 0;
			token = NULL;
			token = strtok(msg.char_mensaje, "|");
			while (token != NULL)
			{
				switch (count)
				{
					case 0:
						codigo=atoi(token);
						printf("COD:%d", codigo); //CODIGO
					break; 
					case 1:
						cantidad=atoi(token);
						printf(" CANT:%d\n", cantidad); //CANTIDAD
					break;
				}
				token = strtok(NULL,"|");
				count++;
		    	}
			inventario[codigo-1].cantidad+=cantidad;
			break;
		default:
			printf("\nEvento sin definir\n");
		break;
	}
	printf("\nINVENTARIO\n");
	for(i=0; i<CANTIDAD_TOTAL;i++)
		printf("COD: %d CANT: %d\n", inventario[i].codigo, inventario[i].cantidad);
	printf("------------------------------\n");
}

	
int main()
{
	codigos*		memoria_codigos = NULL;
	int 			id_memoria_codigos;
	int 			id_cola_mensajes;
	int				i;
	mensaje			msg;

	
	for(i=0;i<CANTIDAD_TOTAL; i++)
	{
		inventario[i].codigo = i+1;
		inventario[i].cantidad = 0;
	} 

	memoria_codigos 	= (codigos*) creo_memoria(id_memoria_codigos, &id_memoria_codigos, CLAVE_BASE );

	id_cola_mensajes 	= creo_id_cola_mensajes(CLAVE_BASE);
	
	cargar_codigos(memoria_codigos); //Carga los codigos en memoria compartida con la descripcion.
	
	while(1)
	{
		recibir_mensaje(id_cola_mensajes, MSG_SUPERMERCADO, &msg);
		procesar_evento(id_cola_mensajes, msg);
	};

	return 0;
}


