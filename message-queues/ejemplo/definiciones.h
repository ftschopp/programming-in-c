#ifndef _DEFINICIONES_H
#define _DEFINICIONES_H
 
#define LARGO_DESCRIPCION 100
#define CANT_SEMAFORO 1
#define DESDE_CODIGO 1
#define HASTA_CODIGO 4
#define CANTIDAD_TOTAL (HASTA_CODIGO-DESDE_CODIGO+1)
#define DESDE_CANTIDAD 1
#define HASTA_CANTIDAD 10 

typedef struct tipo_codigos codigos;
struct tipo_codigos
{
	int codigo;
   	char descripcion[LARGO_DESCRIPCION];
};

typedef struct tipo_stock stock;
struct tipo_stock
{
	int codigo;
	int cantidad;
};


typedef enum
{
	MSG_NADIE,				
	MSG_SUPERMERCADO,		
	MSG_REPOSITOR,			
	MSG_CLIENTE	
}Destinos;

typedef enum
{
	EVT_NINGUNO,
	EVT_CONSULTA_STOCK, //Consulta el stock, lleva como parametro el codigo
	EVT_RESPUESTA_STOCK, //Responde el stock por un codigo pedido CODIGO|CANTIDAD
	EVT_SUMA_STOCK, //Suma 1 al stock, lleva como parametro el codigo
	EVT_RESTA_STOCK
}Eventos;


#endif
