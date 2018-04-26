#include "glob.h"

counter*   g_tareasFinalizadas   = new counter("Tareas_finalizadas");
statistic* g_tiempoEsperaReady   = new bmeans2("Espera_en_fila_Ready", .9);
statistic* g_tiempoServicio      = new bmeans2("Tiempo_servicio", .9);    
statistic* g_tput                = new bmeans2("Troughput", .9);;               
statistic* g_tiempoEsperaMemoria = new bmeans2("Espera_por_fallos_en_memoria", .9);


Registro::Registro(){
	
}

void Registro::print(double tiempo, string quien, string mensaje){
	std::cout << std::setw(TIME_WIDTH) << std::fixed << tiempo << 
		": " << quien << ": " << mensaje << std::endl;
}
