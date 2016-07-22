#include "subscriptor.h"
#include "comun.h"
#include "edsu_comun.h"

/* Variables globales */
static int puertoPthereadT = 0;
static int socketPthereadT = 0;

struct arg_struct {
    int arg1;
    void (*notif_evento)(const char *, const char *);
};

/* Funciones Auxiliares */
void * buclePthread(void * argumentos);
void myitoa(int n, char s[]);
void myreverse(char s[]);

int alta_subscripcion_tema(const char *tema) {
	
	printf("------------------------------------------------------------------------\n");
	printf("[Suscriptor]: Realizamos la funcion: 'alta_subscripcion_tema'\n");
	
	/* Conectamos con el intermediario */
	int nuevoSocket;

	struct hostent * host;
	struct sockaddr_in direccion;

	char * mensaje = "";
	char respuesta[1024];

	nuevoSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	if(nuevoSocket == -1){
		printf("------------------------------------------------------------------------\n");
		fprintf(stderr, "[Suscriptor]: Fallo en 'socket' de 'alta_subscripcion_tema'\n");
		return -1;
	}

	printf("------------------------------------------------------------------------\n");
	printf("[Suscriptor]: Acierto en 'socket'\n");

	host = gethostbyname(getenv("SERVIDOR"));
	memcpy(&direccion.sin_addr.s_addr, host->h_addr, host->h_length);
	direccion.sin_port = htons(atoi(getenv("PUERTO")));
	direccion.sin_family = PF_INET;

	if(connect(nuevoSocket, (struct sockaddr *) &direccion, sizeof(direccion)) == -1){
		printf("------------------------------------------------------------------------\n");
		fprintf(stderr, "[Suscriptor]: Fallo en 'connect' de 'alta_subscripcion_tema'\n");
		return -1;
	}

	printf("------------------------------------------------------------------------\n");
	printf("[Suscriptor]: Acierto en 'connect'\n");

	/* Enviamos el evento al intermediario */
	printf("------------------------------------------------------------------------\n");
	printf("[Suscriptor]: Comprimimos el mensaje\n");

	char * charPuert = malloc(32 * sizeof(char));
	myitoa(puertoPthereadT, charPuert);

	mensaje = malloc(1024 * sizeof(char));
	*mensaje = *strcpy(mensaje,"DarseDeAltaTema");
	*mensaje = *strcat(mensaje," ");
	*mensaje = *strcat(mensaje,tema);
	*mensaje = *strcat(mensaje," ");
	*mensaje = *strcat(mensaje,"vacio");
	*mensaje = *strcat(mensaje," ");
	*mensaje = *strcat(mensaje, charPuert);
	*mensaje = *strcat(mensaje,"\0");

	printf("------------------------------------------------------------------------\n");
	printf("[Suscriptor]: Acierto en 'Comprimimos el mensaje'\n");

	if(write(nuevoSocket, mensaje, (strlen(mensaje) + 1)) == -1){
		printf("------------------------------------------------------------------------\n");
		fprintf(stderr, "[Suscriptor]: Fallo en 'write' de 'alta_subscripcion_tema'\n");
		return -1;
	}

	printf("------------------------------------------------------------------------\n");
	printf("[Suscriptor]: Acierto en 'write'\n");
	printf("[Suscriptor]: He enviado '%s'\n",mensaje);

	shutdown(nuevoSocket, SHUT_RDWR);

	if(read(nuevoSocket, respuesta, sizeof(respuesta)) == -1){
		printf("------------------------------------------------------------------------\n");
		fprintf(stderr, "[Suscriptor]: Fallo en 'read' de 'alta_subscripcion_tema'\n");
		return -1;
	}

	printf("------------------------------------------------------------------------\n");
	printf("[Suscriptor]: Acierto en 'read'\n");

	free(mensaje);
	close(nuevoSocket);
	return 0;
}

int baja_subscripcion_tema(const char *tema) {
	
	printf("------------------------------------------------------------------------\n");
	printf("[Suscriptor]: Realizamos la funcion: 'baja_subscripcion_tema'\n");
	
	/* Conectamos con el intermediario */
	int nuevoSocket;

	struct hostent * host;
	struct sockaddr_in direccion;

	char * mensaje = "";
	char respuesta[1024];

	nuevoSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	if(nuevoSocket == -1){
		printf("------------------------------------------------------------------------\n");
		fprintf(stderr, "[Suscriptor]: Fallo en 'socket' de 'baja_subscripcion_tema'\n");
		return -1;
	}

	printf("------------------------------------------------------------------------\n");
	printf("[Suscriptor]: Acierto en 'socket'\n");

	host = gethostbyname(getenv("SERVIDOR"));
	memcpy(&direccion.sin_addr.s_addr, host->h_addr, host->h_length);
	direccion.sin_port = htons(atoi(getenv("PUERTO")));
	direccion.sin_family = PF_INET;

	if(connect(nuevoSocket, (struct sockaddr *) &direccion, sizeof(direccion)) == -1){
		printf("------------------------------------------------------------------------\n");
		fprintf(stderr, "[Suscriptor]: Fallo en 'connect' de 'baja_subscripcion_tema'\n");
		return -1;
	}

	printf("------------------------------------------------------------------------\n");
	printf("[Suscriptor]: Acierto en 'connect'\n");

	/* Enviamos el evento al intermediario */
	printf("------------------------------------------------------------------------\n");
	printf("[Suscriptor]: Comprimimos el mensaje\n");

	char * charPuert = malloc(32 * sizeof(char));
	myitoa(puertoPthereadT, charPuert);

	mensaje = malloc(1024 * sizeof(char));
	*mensaje = *strcpy(mensaje,"DarseDeBajaTema");
	*mensaje = *strcat(mensaje," ");
	*mensaje = *strcat(mensaje,tema);
	*mensaje = *strcat(mensaje," ");
	*mensaje = *strcat(mensaje,"vacio");
	*mensaje = *strcat(mensaje," ");
	*mensaje = *strcat(mensaje, charPuert);
	*mensaje = *strcat(mensaje,"\0");

	printf("------------------------------------------------------------------------\n");
	printf("[Suscriptor]: Acierto en 'Comprimimos el mensaje'\n");

	if(write(nuevoSocket, mensaje, (strlen(mensaje) + 1)) == -1){
		printf("------------------------------------------------------------------------\n");
		fprintf(stderr, "[Suscriptor]: Fallo en 'write' de 'baja_subscripcion_tema'\n");
		return -1;
	}

	printf("------------------------------------------------------------------------\n");
	printf("[Suscriptor]: Acierto en 'write'\n");
	printf("[Suscriptor]: He enviado '%s'\n",mensaje);

	shutdown(nuevoSocket, SHUT_RDWR);

	if(read(nuevoSocket, respuesta, sizeof(respuesta)) == -1){
		printf("------------------------------------------------------------------------\n");
		fprintf(stderr, "[Suscriptor]: Fallo en 'read' de 'baja_subscripcion_tema'\n");
		return -1;
	}

	printf("------------------------------------------------------------------------\n");
	printf("[Suscriptor]: Acierto en 'read'\n");

	free(mensaje);
	close(nuevoSocket);
	return 0;
}

int inicio_subscriptor(void (*notif_evento)(const char *, const char *),
                void (*alta_tema)(const char *),
                void (*baja_tema)(const char *)) {
	
	printf("------------------------------------------------------------------------\n");
	printf("[Suscriptor]: Realizamos la funcion: 'inicio_subscriptor'\n");
	
	/* Conectamos con el intermediario */
	int nuevoSocket;
	int opcionSetsockopt;

	pthread_t idPthread = 0;

	struct sockaddr_in direccion;

	unsigned int tamanoDireccion;

	nuevoSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	if(nuevoSocket == -1){
		printf("------------------------------------------------------------------------\n");
		fprintf(stderr, "[Suscriptor]: Fallo en 'socket' de 'inicio_subscriptor'\n");
		return -1;
	}

	printf("------------------------------------------------------------------------\n");
	printf("[Suscriptor]: Acierto en 'socket'\n");

	/* Para poder reutilizar el socket para recibir las notificaciones */
	if(setsockopt(nuevoSocket, SOL_SOCKET, SO_REUSEADDR, &opcionSetsockopt, sizeof(opcionSetsockopt)) < 0) {
		printf("------------------------------------------------------------------------\n");
		fprintf(stderr, "[Suscriptor]: Fallo en 'setsockopt' de 'inicio_subscriptor'\n");
		return -1;
	}

	printf("------------------------------------------------------------------------\n");
	printf("[Suscriptor]: Acierto en 'setsockopt'\n");

	/* Asignamos los parametros a la direccion para establecerselos al socket */ 
	direccion.sin_addr.s_addr = INADDR_ANY;
	direccion.sin_port = 0;
	direccion.sin_family = PF_INET;

	if (bind(nuevoSocket, (struct sockaddr *) &direccion, sizeof(direccion)) < 0) {
		printf("------------------------------------------------------------------------\n");
		fprintf(stderr, "[Suscriptor]: Fallo en 'bind' de 'inicio_subscriptor'\n");
		return -1;
	}

	printf("------------------------------------------------------------------------\n");
	printf("[Suscriptor]: Acierto en 'bind'\n");

	if (listen(nuevoSocket, 1024) < 0) {
		printf("------------------------------------------------------------------------\n");
		fprintf(stderr, "[Suscriptor]: Fallo en 'listen' de 'inicio_subscriptor'\n");
		return -1;
	}

	tamanoDireccion = sizeof(direccion);
	getsockname(nuevoSocket, (void *) &direccion, (socklen_t *) &tamanoDireccion);
	puertoPthereadT = ntohs(direccion.sin_port);
	socketPthereadT = nuevoSocket;

	/* Creamos el pthread de escucha de eventos */
	struct arg_struct args = {nuevoSocket,notif_evento};
	struct arg_struct *a = malloc(sizeof(struct arg_struct));
	*a= args;

	pthread_create(&idPthread, NULL, &buclePthread, (void *) a);
	
	return 0;
}

int fin_subscriptor() {
	return 0;
}

void * buclePthread(void * argumentos){

	printf("************************************************************************\n");
	printf("[Suscriptor|Pthread]: Realizamos la funcion del 'Pthread'\n");
	printf("************************************************************************\n");

	void (*notif_evento)(const char *, const char *);

	char respuesta[1024];

	int caractLeidos;
	int socket;
	int socketPthread;

	struct arg_struct * leerParam = argumentos;
	struct sockaddr_in direccionCliente;

	unsigned int tamanoDireccion;

	socket = leerParam->arg1;

	while(1){

		/* Realizamos el accept del Pthread */
		tamanoDireccion = sizeof(direccionCliente);

		printf("************************************************************************\n");
		printf("[Suscriptor|Pthread]: Esperando al 'accept' ...\n");
		printf("************************************************************************\n");
		
		socketPthread = accept((int) socket, (struct sockaddr *) &direccionCliente, &tamanoDireccion);

		if(socketPthread == -1){
			printf("************************************************************************\n");
			fprintf(stderr, "[Suscriptor|Pthread]: Fallo en 'accept' del 'Pthread'\n");
			printf("************************************************************************\n");
			return NULL;
		}

		printf("************************************************************************\n");
		printf("[Suscriptor|Pthread]: Acierto en 'accept'\n");
		printf("************************************************************************\n");

		/* Leemos los eventos del socket */
		caractLeidos = 0;
		
		while((caractLeidos = read(socketPthread, respuesta, sizeof(respuesta))) < 0){
			printf("************************************************************************\n");
			fprintf(stderr, "[Suscriptor|Pthread]: Fallo en 'read' del 'Pthread'\n");
			printf("************************************************************************\n");
			return NULL;
		}

		printf("************************************************************************\n");
		printf("[Suscriptor|Pthread]: Acierto en 'read'\n");
		printf("************************************************************************\n");

		// Tratar el mensaje
		char * tema = malloc(sizeof(char));
		char * noticia = malloc(sizeof(char));

		int j;
		int k = 0;
		int espacios = 0;

		for(j = 0; j < caractLeidos; j++){

			if(respuesta[j] != ' ' && espacios == 0){
				tema[k] = respuesta[j];
				k++;
				tema = realloc(tema, sizeof(char) * k);
			}
			else if(respuesta[j] != ' ' && espacios == 1){
				noticia[k] = respuesta[j];
				k++;
				noticia = realloc(noticia, sizeof(char) * k);
			}
			else{
				tema[k] = '\0';
				espacios++;
				k = 0;
			}
		}
		noticia[k] = '\0';

		notif_evento = leerParam->notif_evento;
		(*notif_evento)(tema,noticia); 

		free(tema);
		free(noticia);

		close(socketPthread);
	}

	return NULL;
}

void myitoa(int n, char s[]){
     int i, sign;
 
     if ((sign = n) < 0)  /* record sign */
         n = -n;          /* make n positive */
     i = 0;
     do {       /* generate digits in reverse order */
         s[i++] = n % 10 + '0';   /* get next digit */
     } while ((n /= 10) > 0);     /* delete it */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     myreverse(s);
 }

void myreverse(char s[]){
     int i, j;
     char c;
 
     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }

