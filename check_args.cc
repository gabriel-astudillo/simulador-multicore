#include "glob.h"
#include "check_args.h"

extern char *optarg;
extern int optind, opterr, optopt;

/*
 *		Funcion que imprime en stdout el uso
 *		del programa.
 */

struct args_t  parametros;

void print_usage(char* name){
	char opciones[] = "-t <nro de tareas> -c <nro de cores>\n";

	char descripcion[] = "Descripción:\n"
	                     "\t-t   cantidad de tareas a simular\n"
						 "\t-c   cantidad de corese del procesador\n";

	printf("Uso: %s %s\n%s\n", name, opciones, descripcion);
}
/* FIN print_usage()*/

void check_args(int argc , char **argv){
	int opcion;

	std::string optString = "t:c:h";

	parametros.totalTareas = 0;
	parametros.totalCores  = 0;

	while ((opcion = getopt (argc, argv, optString.c_str())) != -1){
		switch (opcion) {
			case 't':
					parametros.totalTareas = atoi(optarg);
					break;
			case 'c':
					parametros.totalCores = atoi(optarg);
					break;
			case 'h':
			default:
					print_usage(argv[0]);
					exit(EXIT_FAILURE);
		}
	}

	if ( parametros.totalTareas == 0 || parametros.totalCores == 0) {
		print_usage(argv[0]);
		exit(EXIT_FAILURE);
	}
}
/* FIN check_args()*/
