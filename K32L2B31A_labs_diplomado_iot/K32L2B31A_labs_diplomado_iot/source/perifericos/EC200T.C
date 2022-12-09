/*! @file : EC200T.c
 * @author  Victor Alfonso Fernandez Hoyos
 * @version 1.0.0
 * @date    09/12/2022
 * @brief   Driver para modem EC200T
 * @details
 *
*/
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "EC200T.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef struct _estado_fsm{
	uint8_t anterior;
	uint8_t actual;
}estado_fsm_t;

enum _ec200t_lista_ordendes{
	kORDEN_NO_HAY_EN_EJECUCION=0,
	kORDEN_ENVIAR_MENSAJE_DE_TEXTO,
};

#define EC200T_BYTES_EN_BUFFER	100
#define EC200T_TIEMPO_MAXIMO_ESPERA	5
/*******************************************************************************
 * Private Prototypes
 ******************************************************************************/
void ec200tBorrarBufferRX(void);

/*******************************************************************************
 * External vars
 ******************************************************************************/


/*******************************************************************************
 * Local vars
 ******************************************************************************/
uint8_t ec200t_buffer_tx[EC200T_BYTES_EN_BUFFER];		//almacena los datos a enviar al modem
uint8_t ec200t_buffer_rx[EC200T_BYTES_EN_BUFFER];		//almacena las datos que provienen del modem
uint8_t ec200t_index_buffer_rx = 0;				//apuntador de buffer de datos

estado_fsm_t ec200t_fsm;	//almacena estado actual de la maquina de estados modem EC25

uint32_t ec200t_timeout;	//almacena las veces que han llamado la fsm del EC25 y estimar tiempo

uint8_t ec200t_comando_en_ejecucion;	//almacena ultimo comando enviado para ser ejecutado

//Listado de comando AT disponibles para ser enviados al modem Quectel
const char *ec200t_comandos_at[] = {
	"AT",			//comprueba disponibilidad de dispositivo
	"ATI",			//consulta información del modem
	"AT+CPIN?",		//consulta estado de la simcard
	"AT+CREG?",		//consulta estado de la red celular y tecnología usada en red celular
	"AT+CMGF=1",	//asigna modo texto para enviar mensajes
	"AT+CMGS=\"3107483273\"",//envia mensaje de texto a numero definido
	"Hola este es el diplomado de IoT", //MENSAJE & CTRL+Z
	};

//Lista de respuestas a cada comando AT
const char  *ec200t_repuestas_at[]={
		"OK",		//AT
		"EC25",		//ATI
		"READY",	//AT+CPIN?
		"0,1",		//AT+CREG? = GSM,REGISTERED
		"OK",		//AT+CMGF=1
		">",		//AT+CMGS
		"OK",		//MENSAJE & CTRL+Z
};


/*******************************************************************************
 * Private Source Code
 ******************************************************************************/
//------------------------------------
void ec200tBorrarBufferRX(void){
	uint8_t i;

	//LLenar de ceros buffer que va a recibir datos provenientes del modem
	for(i=0;i<EC200T_BYTES_EN_BUFFER;i++){
		ec200t_buffer_rx[i]=0;
	}

	//borra apuntador de posicion donde se va a almacenar el proximo dato
	//Reset al apuntador
	ec200t_index_buffer_rx=0;
}
//------------------------------------
void ec200tBorrarBufferTX(void){
	uint8_t i;

	//LLenar de ceros buffer que va a recibir datos provenientes del modem
	for(i=0;i<EC200T_BYTES_EN_BUFFER;i++){
		ec200t_buffer_tx[i]=0;
	}
}
//------------------------------------
void waytTimeModem(void) {
	uint32_t tiempo = 0xFFFF;
	do {
		tiempo--;
	} while (tiempo != 0x0000);
}
//------------------------------------
/*******************************************************************************
 * Public Source Code
 ******************************************************************************/
status_t ec200tInicializacion(void){
	ec200t_fsm.anterior=kFSM_INICIO;
	ec200t_fsm.actual=kFSM_INICIO;

	ec200t_timeout=0;	//borra contador de tiempo

	ec200tBorrarBufferTX();	//borrar buffer de transmisión
	ec200tBorrarBufferRX();	//borra buffer de recepción

	ec200t_comando_en_ejecucion=kORDEN_NO_HAY_EN_EJECUCION;	//Borra orden en ejecucipon actual

	return(kStatus_Success);
}
//------------------------------------
status_t ec200tEnviarMensajeDeTexto(uint8_t *mensaje, uint8_t size_mensaje ){

	memcpy(&ec200t_buffer_tx[0],mensaje, size_mensaje);	//copia mensaje a enviar en buffer TX del EC25

	ec200t_comando_en_ejecucion=kORDEN_ENVIAR_MENSAJE_DE_TEXTO;
	ec200t_fsm.actual=kFSM_ENVIANDO_AT;
	return(kStatus_Success);
}
//------------------------------------
uint8_t ec200tPolling(void){
	status_t status;
	uint8_t nuevo_byte_uart;
	uint8_t *puntero_ok=0;	//variable temporal que será usada para buscar respuesta
	switch (ec200t_fsm.actual) {
	case kFSM_INICIO:
		break;

	case kFSM_ENVIANDO_AT:
		ec200tBorrarBufferRX();	//limpia buffer para recibir datos de quectel
		printf("%s\r\n", ec200t_comandos_at[kAT]);	//Envia comando AT

		ec200t_fsm.anterior = ec200t_fsm.actual;		//almacena el estado actual
		ec200t_fsm.actual = kFSM_ESPERANDO_RESPUESTA;	//avanza a esperar respuesta del modem
		ec200t_timeout = 0;	//reset a contador de tiempo
		break;

	case kFSM_ENVIANDO_ATI:
		ec200tBorrarBufferRX();	//limpia buffer para recibir datos de quectel
		printf("%s\r\n", ec200t_comandos_at[kATI]);	//Envia comando AT

		ec200t_fsm.anterior = ec200t_fsm.actual;		//almacena el estado actual
		ec200t_fsm.actual = kFSM_ESPERANDO_RESPUESTA;	//avanza a esperar respuesta del modem
		ec200t_timeout = 0;	//reset a contador de tiempo
		break;

	case kFSM_ENVIANDO_CPIN:
		ec200tBorrarBufferRX();	//limpia buffer para recibir datos de quectel
		printf("%s\r\n", ec200t_comandos_at[kAT_CPIN]);	//Envia comando AT+CPIN?

		ec200t_fsm.anterior = ec200t_fsm.actual;		//almacena el estado actual
		ec200t_fsm.actual = kFSM_ESPERANDO_RESPUESTA;	//avanza a esperar respuesta del modem
		ec200t_timeout = 0;	//reset a contador de tiempo
		break;

	case kFSM_ENVIANDO_CREG:
		ec200tBorrarBufferRX();	//limpia buffer para recibir datos de quectel
		printf("%s\r\n", ec200t_comandos_at[kAT_CREG]);	//Envia comando AT+CREG?

		ec200t_fsm.anterior = ec200t_fsm.actual;		//almacena el estado actual
		ec200t_fsm.actual = kFSM_ESPERANDO_RESPUESTA;	//avanza a esperar respuesta del modem
		ec200t_timeout = 0;	//reset a contador de tiempo
		break;

	case kFSM_ENVIANDO_CMGF:
		ec200tBorrarBufferRX();	//limpia buffer para recibir datos de quectel
		printf("%s\r\n", ec200t_comandos_at[kAT_CMGF_1]);	//Envia comando AT+CMGF=1

		ec200t_fsm.anterior = ec200t_fsm.actual;		//almacena el estado actual
		ec200t_fsm.actual = kFSM_ESPERANDO_RESPUESTA;	//avanza a esperar respuesta del modem
		ec200t_timeout = 0;	//reset a contador de tiempo
		break;

	case kFSM_ENVIANDO_CMGS:
		ec200tBorrarBufferRX();	//limpia buffer para recibir datos de quectel
		printf("%s\r\n", ec200t_comandos_at[kAT_CMGS]);	//Envia comando AT+CMGS="3003564960"

		ec200t_fsm.anterior = ec200t_fsm.actual;		//almacena el estado actual
		ec200t_fsm.actual = kFSM_ESPERANDO_RESPUESTA;	//avanza a esperar respuesta del modem
		ec200t_timeout = 0;	//reset a contador de tiempo
		break;

	case kFSM_ENVIANDO_MENSAJE_TXT:
		printf("%s\r\n%c", ec200t_buffer_tx,0x1A);	//Envia mensaje de texto incluido  CTRL+Z (0x1A)
		ec200t_fsm.anterior = ec200t_fsm.actual;		//almacena el estado actual
		ec200t_fsm.actual = kFSM_ESPERANDO_RESPUESTA;	//avanza a esperar respuesta del modem
		ec200t_timeout = 0;	//reset a contador de tiempo
		break;

	case kFSM_RESULTADO_ERROR:
		printf("EC200T ERROR\r\n");
		ec200tInicializacion();
		break;

	case kFSM_RESULTADO_EXITOSO:
		break;

	case kFSM_ESPERANDO_RESPUESTA:
		ec200t_timeout++;	//incrementa contador de tiempo

		//Busca si llegaron nuevos datos desde modem mientras esperaba
		while (lpUart0CuantosDatosHayEnBuffer() > 0) {
			status = lpUart0LeerByteDesdeBuffer(&nuevo_byte_uart);
			if (status == kStatus_Success) {
				//reinicia contador de tiempo
				ec200t_timeout=0;
				//almacena dato en buffer rx de quectel
				ec200t_buffer_rx[ec200t_index_buffer_rx] = nuevo_byte_uart;
				//incrementa apuntador de datos en buffer de quectel
				ec200t_index_buffer_rx++;
			}
		}

		//pregunta si el tiempo de espera supera el configurado
		if(ec200t_timeout>EC200T_TIEMPO_MAXIMO_ESPERA){
			//busca la respuesta indicada dependiendeo de cual comando AT se le había enviado al modem
			switch(ec200t_fsm.anterior){
			case kFSM_ENVIANDO_AT:
				//Busca palabra EC25 en buffer rx de quectel
				puntero_ok = (uint8_t*) (strstr((char*) (&ec200t_buffer_rx[0]),(char*) (ec200t_repuestas_at[kAT])));

				if(puntero_ok!=0x00){
					//Si la respuesta es encontrada, se avanza al siguiente estado
					ec200t_fsm.anterior = ec200t_fsm.actual;		//almacena el estado actual
					ec200t_fsm.actual = kFSM_ENVIANDO_ATI;		//avanza a enviar nuevo comando al modem
				}else{
					//Si la respuesta es incorrecta, se queda en estado de error
					//No se cambia (ec200t_fsm.anterior) para mantener en que comando AT fue que se generó error
					ec200t_fsm.actual = kFSM_RESULTADO_ERROR;		//se queda en estado de error
				}

				break;
			case kFSM_ENVIANDO_ATI:
				//Busca palabra EC25 en buffer rx de quectel
				puntero_ok = (uint8_t*) (strstr((char*) (&ec200t_buffer_rx[0]),
						(char*) (ec200t_repuestas_at[kATI])));

				if(puntero_ok!=0x00){
					//Si la respuesta es encontrada, se avanza al siguiente estado
					ec200t_fsm.anterior = ec200t_fsm.actual;		//almacena el estado actual
					ec200t_fsm.actual = kFSM_ENVIANDO_CPIN;		//avanza a enviar nuevo comando al modem
				}else{
					//Si la respuesta es incorrecta, se queda en estado de error
					//No se cambia (ec200t_fsm.anterior) para mantener en que comando AT fue que se generó error
					ec200t_fsm.actual = kFSM_RESULTADO_ERROR;		//se queda en estado de error
				}
				break;
			case kFSM_ENVIANDO_CPIN:
				//Busca palabra EC25 en buffer rx de quectel
				puntero_ok = (uint8_t*) (strstr((char*) (&ec200t_buffer_rx[0]),
						(char*) (ec200t_repuestas_at[kAT_CPIN])));

				if(puntero_ok!=0x00){
					//Si la respuesta es encontrada, se avanza al siguiente estado
					ec200t_fsm.anterior = ec200t_fsm.actual;		//almacena el estado actual
					ec200t_fsm.actual = kFSM_ENVIANDO_CREG;		//avanza a enviar nuevo comando al modem
				}else{
					//Si la respuesta es incorrecta, se queda en estado de error
					//No se cambia (ec200t_fsm.anterior) para mantener en que comando AT fue que se generó error
					ec200t_fsm.actual = kFSM_RESULTADO_ERROR;		//se queda en estado de error
				}
				break;
			case kFSM_ENVIANDO_CREG:
				//Busca palabra EC25 en buffer rx de quectel
				puntero_ok = (uint8_t*) (strstr((char*) (&ec200t_buffer_rx[0]),
						(char*) (ec200t_repuestas_at[kAT_CREG])));

				if(puntero_ok!=0x00){
					//Si la respuesta es encontrada, se avanza al siguiente estado
					ec200t_fsm.anterior = ec200t_fsm.actual;		//almacena el estado actual
					ec200t_fsm.actual = kFSM_ENVIANDO_CMGF;		//avanza a enviar nuevo comando al modem
				}else{
					//Si la respuesta es incorrecta, se queda en estado de error
					//No se cambia (ec200t_fsm.anterior) para mantener en que comando AT fue que se generó error
					ec200t_fsm.actual = kFSM_RESULTADO_ERROR;		//se queda en estado de error
				}
				break;
			case kFSM_ENVIANDO_CMGF:
				//Busca palabra EC25 en buffer rx de quectel
				puntero_ok = (uint8_t*) (strstr((char*) (&ec200t_buffer_rx[0]),
						(char*) (ec200t_repuestas_at[kAT_CMGF_1])));

				if(puntero_ok!=0x00){
					//Si la respuesta es encontrada, se avanza al siguiente estado
					ec200t_fsm.anterior = ec200t_fsm.actual;		//almacena el estado actual
					ec200t_fsm.actual = kFSM_ENVIANDO_CMGS;		//avanza a enviar nuevo comando al modem
				}else{
					//Si la respuesta es incorrecta, se queda en estado de error
					//No se cambia (ec200t_fsm.anterior) para mantener en que comando AT fue que se generó error
					ec200t_fsm.actual = kFSM_RESULTADO_ERROR;		//se queda en estado de error
				}
				break;

			case kFSM_ENVIANDO_CMGS:
				//Busca palabra EC25 en buffer rx de quectel
				puntero_ok = (uint8_t*) (strstr((char*) (&ec200t_buffer_rx[0]),
						(char*) (ec200t_repuestas_at[kAT_CMGS])));

				if(puntero_ok!=0x00){
					//Si la respuesta es encontrada, se avanza al siguiente estado
					ec200t_fsm.anterior = ec200t_fsm.actual;		//almacena el estado actual
					ec200t_fsm.actual = kFSM_ENVIANDO_MENSAJE_TXT;		//avanza a enviar nuevo comando al modem
				}else{
					//Si la respuesta es incorrecta, se queda en estado de error
					//No se cambia (ec200t_fsm.anterior) para mantener en que comando AT fue que se generó error
					ec200t_fsm.actual = kFSM_RESULTADO_ERROR;		//se queda en estado de error
				}
				break;

			case kFSM_ENVIANDO_MENSAJE_TXT:
				//Busca palabra EC25 en buffer rx de quectel
				puntero_ok = (uint8_t*) (strstr((char*) (&ec200t_buffer_rx[0]),
						(char*) (ec200t_repuestas_at[kAT_TEXT_MSG_END])));

				if(puntero_ok!=0x00){
					//Si la respuesta es encontrada, se avanza al siguiente estado
					ec200t_fsm.anterior = ec200t_fsm.actual;		//almacena el estado actual
					ec200t_fsm.actual = kFSM_RESULTADO_EXITOSO;		//avanza a enviar nuevo comando al modem
				}else{
					//Si la respuesta es incorrecta, se queda en estado de error
					//No se cambia (ec200t_fsm.anterior) para mantener en que comando AT fue que se generó error
					ec200t_fsm.actual = kFSM_RESULTADO_ERROR;		//se queda en estado de error
				}
				break;

			default:
				//para evitar bloqueos, se reinicia la fsm en caso de entrar a un estado ilegal
				ec200tInicializacion();
				break;
			}
		}
		break;
	default:
		//para evitar bloqueos, se reinicia la fsm en caso de entrar a un estado ilegal
		ec200tInicializacion();
		break;
	}

	return(ec200t_fsm.actual);
}


status_t detectarModemQuectel(void){
	uint8_t nuevo_byte_uart;
	status_t status;
	uint8_t *puntero_ok=0;
	uint32_t contador_tiempo_ms=0;

	ec200tBorrarBufferRX();	//limpia buffer para recibir datos de quectel

	//envia comando ATI a modem quectel
	printf("%s\r\n",ec200t_comandos_at[kATI]);

	//Recibir la respuesta es automatica por IRq de UART0

	//peeeero Sacar los datos del buffer circular y llevarlos al buffer del modem
	//es tarea del proceso interno

	do {
		waytTimeModem();
		contador_tiempo_ms++;

		if (lpUart0CuantosDatosHayEnBuffer() > 0) {
			status = lpUart0LeerByteDesdeBuffer(&nuevo_byte_uart);
			if (status == kStatus_Success) {
				//reinicia contador de tiempo
				contador_tiempo_ms=0;
				//almacena dato en buffer rx de quectel
				ec200t_buffer_rx[ec200t_index_buffer_rx] = nuevo_byte_uart;
				//incrementa apuntador de datos en buffer de quectel
				ec200t_index_buffer_rx++;
			}
		}
	} while (contador_tiempo_ms < 5);

	//Busca palabra EC25 en buffer rx de quectel
	puntero_ok=(uint8_t *)(strstr((char*)(&ec200t_buffer_rx[0]),(char *)(ec200t_repuestas_at[kATI])));

	if(puntero_ok!=0){
		return(kStatus_Success);
	}else{
		return(kStatus_Fail);
	}
}

status_t fsmModemQuectelEC200T(void){
	return(kStatus_Fail);
}
