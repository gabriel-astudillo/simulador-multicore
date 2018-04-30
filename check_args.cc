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
	char opciones[] = "-t <nro de tareas> -c <nro de cores> [-v]\n";

	char descripcion[] = "Descripción:\n"
	                     "\t-t   cantidad de Tareas a simular\n"
						 "\t-s   taSa de llegada: numero medio de tareas que llegan por unidad de tiempo\n"
						 "\t-c   cantidad de Cores del procesador\n"						 
						 "\t-v   Muestra en pantalla el registro de la simulacion\n";

	printf("Uso: %s %s\n%s\n", name, opciones, descripcion);
}
/* FIN print_usage()*/

void check_args(int argc , char **argv){
	int opcion;

	std::string optString = "t:c:s:hv";

	parametros.totalTareas = 0;
	parametros.totalCores  = 0;
	parametros.tasaLlegada = 0.0;
	parametros.t_registro  = REG_QUIET;

	while ((opcion = getopt (argc, argv, optString.c_str())) != -1){
		switch (opcion) {
			case 't':
					parametros.totalTareas = atoi(optarg);
					break;
			case 'c':
					parametros.totalCores = atoi(optarg);
					break;
			case 'v':
					parametros.t_registro = REG_VERBOSE;
					break;
			case 's':
					parametros.tasaLlegada = atof(optarg);
					break;
			case 'h':
			default:
					print_usage(argv[0]);
					exit(EXIT_FAILURE);
		}
	}

	if ( parametros.totalTareas == 0 || parametros.totalCores == 0 ||
	     parametros.tasaLlegada == 0.0 ) {
		print_usage(argv[0]);
		exit(EXIT_FAILURE);
	}
}
/* FIN check_args()*/
