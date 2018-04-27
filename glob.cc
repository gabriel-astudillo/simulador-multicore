#include "glob.h"

/*
*	Indicadores
*/

counter*   g_tareasFinalizadas   = new counter("Tareas_finalizadas");
statistic* g_tiempoEsperaReady   = new mean("Espera_en_fila_Ready", .95);
mean*      g_tiempoServicio      = new mean("Tiempo_servicio", .95);    
statistic* g_tput                = new mean("Troughput", .95);;               
statistic* g_tiempoEsperaMemoria = new mean("Espera_por_fallos_en_memoria", .95);


/*
*	Tiempos de transferencia
*	entre las distintas memorias
*/
const double TR_L2_L1   = 2.0;
const double TR_RAM_L2  = 4.0;

/*
*	Clase Registro
*/
Registro::Registro(){
	tipo = REG_VERBOSE;
}

Registro::Registro(t_register _tipo){
	tipo = _tipo;
}

void Registro::print(double tiempo, string quien, string mensaje){
	if( tipo == REG_VERBOSE){
		std::cout << std::setw(TIME_WIDTH) << std::fixed << tiempo << 
			": " << quien << ": " << mensaje << std::endl;
	}
	
}
