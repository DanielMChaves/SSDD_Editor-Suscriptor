#include "editor.h"
#include "comun.h"
#include "edsu_comun.h"

int generar_evento(const char *tema, const char *valor) {

	printf("------------------------------------------------------------------------\n");
	printf("[Editor]: Realizamos la funcion: 'generar_evento'\n");
	
	/* Conectamos con el intermediario */
	int caracteresLeidos;
	int nuevoSocket;

	struct hostent * host;
	struct sockaddr_in direccion;

	char * mensaje = "";
	char respuesta[1024];

	nuevoSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	if(nuevoSocket == -1){
		printf("------------------------------------------------------------------------\n");
		fprintf(stderr, "[Editor]: Fallo en 'socket' de 'generar_evento'\n");
		return -1;
	}

	printf("------------------------------------------------------------------------\n");
	printf("[Editor]: Acierto en 'socket'\n");

	host = gethostbyname(getenv("SERVIDOR"));
	memcpy(&direccion.sin_addr.s_addr, host->h_addr, host->h_length);
	direccion.sin_port = htons(atoi(getenv("PUERTO")));
	direccion.sin_family = PF_INET;

	if(connect(nuevoSocket, (struct sockaddr *) &direccion, sizeof(direccion)) == -1){
		printf("------------------------------------------------------------------------\n");
		fprintf(stderr, "[Editor]: Fallo en 'connect' de 'generar_evento'\n");
		return -1;
	}

	printf("------------------------------------------------------------------------\n");
	printf("[Editor]: Acierto en 'connect'\n");

	/* Enviamos el evento al intermediario */
	printf("------------------------------------------------------------------------\n");
	printf("[Editor]: Comprimimos el mensaje\n");

	mensaje = malloc(64 * sizeof(char));
	*mensaje = *strcat(mensaje,"GenerarEvento");
	*mensaje = *strcat(mensaje," ");
	*mensaje = *strcat(mensaje,tema);
	*mensaje = *strcat(mensaje," ");
	*mensaje = *strcat(mensaje,valor);
	*mensaje = *strcat(mensaje,"\0");

	printf("------------------------------------------------------------------------\n");
	printf("[Editor]: Acierto en 'Comprimimos el mensaje'\n");

	if(write(nuevoSocket, mensaje, (strlen(mensaje)+1)) == -1){
		printf("------------------------------------------------------------------------\n");
		fprintf(stderr, "[Editor]: Fallo en 'write' de 'generar_evento'\n");
		return -1;
	}

	printf("------------------------------------------------------------------------\n");
	printf("[Editor]: Acierto en 'write'\n");
	printf("[Editor]: He enviado '%s'\n",mensaje);

	shutdown(nuevoSocket, SHUT_RDWR);

	if((caracteresLeidos = read(nuevoSocket, respuesta, sizeof(respuesta))) == -1){
		printf("------------------------------------------------------------------------\n");
		fprintf(stderr, "[Editor]: Fallo en 'read' de 'generar_evento'\n");
		return -1;
	}

	printf("------------------------------------------------------------------------\n");
	printf("[Editor]: Acierto en 'read'\n");

	free(mensaje);
	close(nuevoSocket);
	return 0;
}

/* solo para la version avanzada */
int crear_tema(const char *tema) {
	return 0;
}

/* solo para la version avanzada */
int eliminar_tema(const char *tema) {
	return 0;
}

