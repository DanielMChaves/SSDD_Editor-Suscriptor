/* ./intermediario numeroPuerto listaTemas */

#include "comun.h"

/* Creacion de la estructura de datos */
typedef struct suscriptor {
	struct sockaddr_in * socketCliente;
} suscriptor;

typedef suscriptor * nuevoSuscriptor;

typedef struct tema {
	char * nombre;
	char * ultimaNoticia;
	int numSuscriptores;
	nuevoSuscriptor * listaSuscriptores;
} tema;

typedef struct listaTemas {
	tema ** temas;
	int numTemas;
} listaTemas;

/* Funciones Auxiliares - Cabeceras */

/* Lee el fichero de temas y los mete en la estructura de datos */
listaTemas * init_temas(const char * ficheroTemas);
/* Mete un tema en la estructura de datos */
tema * meter_tema(const char * tem);
/* Imprime todos los temas leidos del fichero */
void imprimir_temas(const listaTemas* temas);
/* Imprime el numero de suscriptores que tiene el tema
pasado por parametro */
void imprimir_tema(const tema * tema);
/* Mete un nuevo evento en la estructura de datos y se lo
notifica a todos sus suscriptores */
void procesar_generar_evento(char * tema, const listaTemas * lista, const char * noticia);
/* Damos de alta a un suscriptor a un tema */
int procesar_alta(char * tema, const listaTemas * lista, struct sockaddr_in * dirCliente, const char * puerto);
/* Damos de baja a un suscriptor a un tema */
int procesar_baja(char * tema, const listaTemas * lista, struct sockaddr_in * dirCliente, const char * puerto);
/* Envia la ultima noticia de un evento a los suscriptores */
int enviar_evento(const char * tema, const char * noticia, struct sockaddr_in * suscriptor);
/* Metodo strcmp especializado */
int my_strcmp(char s1[], char s2[]);

/* Funciones Auxiliares - Implementacion */
listaTemas * init_temas(const char * ficheroTemas){

	/* Estructura vacia */
	listaTemas * listaMalloc = malloc(sizeof(ficheroTemas));
	listaMalloc->temas = NULL;
	listaMalloc->numTemas = 0;

	/* Tratamiento del fichero */
	FILE* fichero = fopen(ficheroTemas,"r");

	if(fichero == NULL){
		printf("------------------------------------------------------------------------\n");
		fprintf(stderr, "Fallo en 'fopen' de init_temas\n");
		exit(1);
	}

	printf("------------------------------------------------------------------------\n");
	printf("[Intermediario]: Acierto en 'fopen'\n");

	char tema[50];
	int i = 0;

	while (fgets(tema,50,fichero) != NULL){

 		/* Introducimos el tema en la estructura de datos */
 		if (listaMalloc->numTemas % 5 == 0) {
			listaMalloc->temas = realloc(listaMalloc->temas,(listaMalloc->numTemas + 5) * sizeof(char*));
		}

 		listaMalloc->temas[listaMalloc->numTemas] = meter_tema(tema);
 		listaMalloc->numTemas++;
 		i++;	
 	}

 	fclose(fichero);

 	return listaMalloc;
}

tema * meter_tema(const char * tem){

	tema * temaMalloc = malloc(sizeof(tem));

	int i = 0;

	while(tem[i] != '\0'){
		if (i % 5 == 0)
			temaMalloc->nombre = realloc(temaMalloc->nombre, (i + 5) * sizeof(char));
		temaMalloc->nombre[i] = tem[i];
		i++;
	}

	temaMalloc->nombre[i] = '\0';
	temaMalloc->ultimaNoticia = NULL;
	temaMalloc->numSuscriptores = 0;
	temaMalloc->listaSuscriptores = malloc(sizeof(nuevoSuscriptor *));

	return temaMalloc;
}

void imprimir_temas(const listaTemas * temas) {
	
	int i = 0;

	printf("------------------------------------------------------------------------\n");
	printf("[Intermediario]: Hay %d temas en el fichero 'temas.txt'\n", temas->numTemas);

	for (i = 0; i < temas->numTemas; i++) {
		printf("[Intermediario]: El tema leido es: %s", temas->temas[i]->nombre);
	}
}

void imprimir_tema(const tema * tema) {
	printf("[Intermediario]: Hay %d suscriptores en el tema: %s",tema->numSuscriptores,tema->nombre);
}

void procesar_generar_evento(char * tema, const listaTemas * lista, const char * noticia){
	
	printf("------------------------------------------------------------------------\n");
	printf("[Intermediario]: Realizamos la funcion: 'procesar_generar_evento'\n");

	/* Buscamos el tema que hemos recibido */
	int encontrado = 1;
	int i;
	int j;
	int numSus;

	//*tema = *strcat(tema,"\n");

	for(i = 0; i < lista->numTemas && encontrado == 1; i++){
		if(my_strcmp(lista->temas[i]->nombre,tema) == 0){
			encontrado = 0;
		}
	}

	if(encontrado == 1){
		printf("------------------------------------------------------------------------\n");
		fprintf(stderr, "[Intermediario]: Fallo al buscar un tema en 'procesar_alta'\n");
		fprintf(stderr, "[Intermediario]: El tema '%s' no se encuentra disponible\n",tema);
		return;
	}

	i--;
	numSus = lista->temas[i]->numSuscriptores;

	for(j = 0; j < numSus; j++){
		enviar_evento(tema,noticia,lista->temas[i]->listaSuscriptores[j]->socketCliente);
	}
}

int procesar_alta(char * tema, const listaTemas * lista, struct sockaddr_in * dirCliente, const char * puerto){

	printf("------------------------------------------------------------------------\n");
	printf("[Intermediario]: Realizamos la funcion: 'procesar_alta'\n");

	/* Buscamos el tema que hemos recibido */
	int encontrado = 1;
	int estasDado = 1;
	int i;
	int j;
	int nuevoPuerto = atoi(puerto);
	int numSus;

	//*tema = *strcat(tema,"\n");

	for(i = 0; i < lista->numTemas && encontrado == 1; i++){
		if(my_strcmp(lista->temas[i]->nombre,tema) == 0){
			encontrado = 0;
		}
	}

	if(encontrado == 1){
		printf("------------------------------------------------------------------------\n");
		fprintf(stderr, "[Intermediario]: Fallo al buscar un tema en 'procesar_alta'\n");
		fprintf(stderr, "[Intermediario]: El tema '%s' no se encuentra disponible\n",tema);
		return -1;
	}

	i--;

	/* Meter el socket en el struct */
	numSus = lista->temas[i]->numSuscriptores;

	if(numSus != 0){
		for(j = 0; j < numSus && estasDado == 1; j++){
			if ((lista->temas[i]->listaSuscriptores[j]->socketCliente->sin_port == htons(nuevoPuerto)) && 
				lista->temas[i]->listaSuscriptores[j]->socketCliente->sin_addr.s_addr == dirCliente->sin_addr.s_addr){
				return -1;
			}
		}
	}

	lista->temas[i]->numSuscriptores++;

	lista->temas[i]->listaSuscriptores[numSus] = malloc(sizeof(64));
	lista->temas[i]->listaSuscriptores[numSus]->socketCliente = malloc(sizeof(64));

	memcpy((void *) lista->temas[i]->listaSuscriptores[numSus]->socketCliente, (void *) dirCliente,sizeof(struct sockaddr_in));
 	lista->temas[i]->listaSuscriptores[numSus]->socketCliente->sin_port = htons(nuevoPuerto);
	lista->temas[i]->listaSuscriptores[numSus]->socketCliente->sin_family = PF_INET;

	imprimir_tema(lista->temas[i]);

	return 0;
}

int procesar_baja(char * tema, const listaTemas * lista, struct sockaddr_in * dirCliente, const char * puerto){

	printf("------------------------------------------------------------------------\n");
	printf("[Intermediario]: Realizamos la funcion: 'procesar_baja'\n");

	/* Buscamos el tema que hemos recibido */
	int encontrado = 1;
	int i;
	int j;
	int k;
	int nuevoPuerto = atoi(puerto);
	int numSus;

	//*tema = *strcat(tema,"\n");

	for(i = 0; i < lista->numTemas && encontrado == 1; i++){
		if(my_strcmp(lista->temas[i]->nombre,tema) == 0){
			encontrado = 0;
		}
	}

	if(encontrado == 1){
		printf("------------------------------------------------------------------------\n");
		fprintf(stderr, "[Intermediario]: Fallo al buscar un tema en 'procesar_baja'\n");
		fprintf(stderr, "[Intermediario]: El tema '%s' no se encuentra disponible\n",tema);
		return -2;
	}

	i--;
	numSus = lista->temas[i]->numSuscriptores;
	
	if(numSus != 0){

		for(j = 0; j < numSus; j++){
			if ((lista->temas[i]->listaSuscriptores[j]->socketCliente->sin_port == htons(nuevoPuerto)) && 
				lista->temas[i]->listaSuscriptores[j]->socketCliente->sin_addr.s_addr == dirCliente->sin_addr.s_addr){

				for(k = j; k < numSus - 1; k++){
					lista->temas[i]->listaSuscriptores[k] = lista->temas[i]->listaSuscriptores[k+1];
				}

				lista->temas[i]->numSuscriptores--;
			}
		}
	}
	
	return 0;
}

int enviar_evento(const char * tema, const char * noticia, struct sockaddr_in * suscriptor){

	printf("------------------------------------------------------------------------\n");
	printf("[Intermediario]: Realizamos la funcion: 'enviar_evento'\n");

	int socketCliente = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (socketCliente == -1) {
		printf("------------------------------------------------------------------------\n");
		fprintf(stderr, "[Intermediario]: Fallo en 'socket' de 'enviar_evento'\n");
		return -1;
	}

	printf("------------------------------------------------------------------------\n");
	printf("[Intermediario]: Acierto en 'socket'\n");

	if (connect(socketCliente, (struct sockaddr *) suscriptor, sizeof(struct sockaddr_in)) == -1) {
		printf("------------------------------------------------------------------------\n");
		fprintf(stderr, "[Intermediario]: Fallo en 'connect' de 'enviar_evento'\n");
		return -1;
	}

	printf("------------------------------------------------------------------------\n");
	printf("[Intermediario]: Acierto en 'connect'\n");

	printf("------------------------------------------------------------------------\n");
	printf("[Intermediario]: Comprimimos el mensaje\n");

	char * mensaje = malloc(1024 * sizeof(char));
	*mensaje = *strcpy(mensaje,tema);
	*mensaje = *strcat(mensaje," ");
	*mensaje = *strcat(mensaje,noticia);
	*mensaje = *strcat(mensaje,"\0");

	if (write(socketCliente, mensaje, strlen(mensaje)) == -1) {
		printf("------------------------------------------------------------------------\n");
		fprintf(stderr, "[Intermediario]: Fallo en 'write' de 'enviar_evento'\n");
		return -1;
	}

	printf("------------------------------------------------------------------------\n");
	printf("[Intermediario]: Acierto en 'write'\n");

	close(socketCliente);
	return 0;
}

int my_strcmp(char s1[], char s2[]){   
    int i = 0;
    while ( s1[i] != '\n' )
    {
        if( s2[i] == '\n' ) { return 1; }
        else if( s1[i] < s2[i] ) { return -1; }
        else if( s1[i] > s2[i] ) { return 1; }
        i++;
    }   
    return 0;
}

int main(int argc, char * argv[]) {

	printf("------------------------------------------------------------------------\n");
	printf("[Intermediario]: Inicializamos el proceso\n");

	/* Comprobacion del numero de argumentos */
	if (argc != 3) {
		printf("------------------------------------------------------------------------\n");
		fprintf(stderr, "Uso: %s puerto fichero_temas\n", argv[0]);
		return -1;
	}

	/* Inicializacion de los temas */
	listaTemas * lista = init_temas(argv[2]);
	imprimir_temas(lista);

	/* Declaracion de variables para los sockets */
	char pregunta[1024];
	char * respuesta = "";

	int caracteresLeidos;
	int notificacion;
	int nuevoSocket;
	int puerto = atoi(argv[1]);
	int socketConectado;

	struct sockaddr_in direccion;
	struct sockaddr_in direccionCliente;

	unsigned int tamanoDireccion;

	/* Creacion de los sockets */
	nuevoSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(nuevoSocket == -1){
		fprintf(stderr, "[Intermediario]: Fallo en 'socket' del main\n");
		return -1;
	}

	printf("------------------------------------------------------------------------\n");
	printf("[Intermediario]: Acierto en 'socket'\n");

	int opcionSersockopt = 1;

	if(setsockopt(nuevoSocket, SOL_SOCKET, SO_REUSEADDR, &opcionSersockopt, sizeof(opcionSersockopt)) == -1){
		printf("------------------------------------------------------------------------\n");
		fprintf(stderr, "[Intermediario]: Fallo en 'setsockopt' del main\n");
		return -1;
	}

	printf("------------------------------------------------------------------------\n");
	printf("[Intermediario]: Acierto en 'setsockopt'\n");

	/* Modificamos la sockaddr_in */
	direccion.sin_addr.s_addr = INADDR_ANY;
	direccion.sin_port = htons(puerto);
	direccion.sin_family = PF_INET;

	/* bind y listen del socket conectado */
	if(bind(nuevoSocket, (struct sockaddr *) &direccion, sizeof(struct sockaddr_in)) == -1){
		printf("------------------------------------------------------------------------\n");
		fprintf(stderr, "[Intermediario]: Fallo en 'bind' del main\n");
		return -1;
	}

	printf("------------------------------------------------------------------------\n");
	printf("[Intermediario]: Acierto en 'bind'\n");

	if(listen(nuevoSocket, 1024) == -1){
		printf("------------------------------------------------------------------------\n");
		fprintf(stderr, "[Intermediario]: Fallo en 'listen' del main\n");
		return -1;
	}

	printf("------------------------------------------------------------------------\n");
	printf("[Intermediario]: Acierto en 'listen'\n");

	/* Bucle para procesar peticiones */
	while(1){

		printf("------------------------------------------------------------------------\n");
		printf("[Intermediario]: Bucle de procesamiento de peticiones\n");

		/* Aceptamos las conexiones a traves del socket */
		tamanoDireccion = sizeof(direccionCliente);

		printf("------------------------------------------------------------------------\n");
		printf("[Intermediario]: Esperando al 'accept' ...\n");

		socketConectado = accept(nuevoSocket, (struct sockaddr *) &direccionCliente, &tamanoDireccion);
		if(socketConectado == -1){
			printf("------------------------------------------------------------------------\n");
			fprintf(stderr, "[Intermediario]: Fallo en 'accept' del main\n");
			return -1;
		}

		printf("------------------------------------------------------------------------\n");
		printf("[Intermediario]: Acierto en 'accept'\n");

		/* Lectura en el socket */
		if((caracteresLeidos = read(socketConectado, pregunta, sizeof(pregunta))) == -1){
			printf("------------------------------------------------------------------------\n");
			fprintf(stderr, "[Intermediario]: Fallo en 'read' del main\n");
			return -1;
		}

		printf("------------------------------------------------------------------------\n");
		printf("[Intermediario]: Acierto en 'read'\n");
		printf("[Intermediario]: He recibido '%s'\n", pregunta);

		/* Procesamos la informacion recibida */
		char * accion = malloc(sizeof(char));
		char * tema = malloc(sizeof(char));
		char * noticia = malloc(sizeof(char));
		char * puertoMsg = malloc(sizeof(char));

		int j;
		int k = 0;
		int espacios = 0;

		for(j = 0; j < caracteresLeidos; j++){

			if(pregunta[j] != ' ' && espacios == 0){
				accion[k] = pregunta[j];
				k++;
				accion = realloc(accion, sizeof(char) * k);
			}
			else if(pregunta[j] != ' ' && espacios == 1){
				tema[k] = pregunta[j];
				k++;
				tema = realloc(tema, sizeof(char) * k);
			}
			else if(pregunta[j] != ' ' && espacios == 2){
				noticia[k] = pregunta[j];
				k++;
				noticia = realloc(noticia, sizeof(char) * k);
			}
			else if(pregunta[j] != ' ' && espacios == 3){
				puertoMsg[k] = pregunta[j];
				k++;
				puertoMsg = realloc(puertoMsg, sizeof(char) * k);
			}
			else{
				espacios++;
				if(espacios == 1) accion[k] = '\0'; 
				else if(espacios == 2) tema[k] = '\0'; 
				else if(espacios == 3) noticia[k] = '\0'; 

				k = 0;
			}

			puertoMsg[k] = '\0'; 
		}
		
		/* Solo funciona cuando el tema y la noticia tienen 5 o mas caracteres */
		printf("------------------------------------------------------------------------\n");
		printf("[Intermediario]: He recibido como accion: '%s'\n",accion);
		printf("[Intermediario]: He recibido como tema: '%s'\n",tema);
		printf("[Intermediario]: He recibido como noticia: '%s'\n",noticia);
		printf("[Intermediario]: He recibido como puerto: '%s'\n",puertoMsg);

		if((getsockname(socketConectado,(struct sockaddr *) &direccionCliente, &tamanoDireccion)) == -1){
			printf("------------------------------------------------------------------------\n");
			fprintf(stderr, "[Intermediario]: Fallo en 'getsockname' del main\n");
			return -1;
		}

		notificacion = 0;

		/* Diferenciamos la peticion y la realizamos */
		if(strstr(accion,"GenerarEvento")){
			printf("------------------------------------------------------------------------\n");
			printf("[Intermediario]: Accion a realizar: '%s'\n",accion);
			procesar_generar_evento(tema, lista, noticia);
		}
		else if(strstr(accion,"DarseDeAltaTema")){
			printf("------------------------------------------------------------------------\n");
			printf("[Intermediario]: Accion a realizar: '%s'\n",accion);
			notificacion = procesar_alta(tema, lista, &direccionCliente, puertoMsg);
		}
		else if(strstr(accion,"DarseDeBajaTema")){
			printf("------------------------------------------------------------------------\n");
			printf("[Intermediario]: Accion a realizar: '%s'\n",accion);
			notificacion = procesar_baja(tema, lista, &direccionCliente, puertoMsg);
		}
		else{
			printf("------------------------------------------------------------------------\n");
			fprintf(stderr,"[Intermediario]: Fallo en identificar la 'accion' del main\n");
			return -1;
		}

		if(notificacion == -1){
			printf("------------------------------------------------------------------------\n");
			printf("[Intermediario]: Ya te has suscrito a '%s'\n",tema);
		}

		if(notificacion == -2){
			printf("------------------------------------------------------------------------\n");
			printf("[Intermediario]: No te puedes dar de baja del tema: '%s' porque no te has suscrito previamente\n",tema);
		}

		free(accion);
		free(tema);
		free(noticia);
		free(puertoMsg);

		/* Escritura en el socket */
		if(write(socketConectado, respuesta, strlen(respuesta)) == -1){
			printf("------------------------------------------------------------------------\n");
			fprintf(stderr, "[Intermediario]: Fallo en 'write' del main\n");
			return -1;
		}

		printf("------------------------------------------------------------------------\n");
		printf("[Intermediario]: Acierto en 'write'\n");

		close(socketConectado);
	}

	/* Cerramos el socket */
	close(nuevoSocket);

	return 0;
}
