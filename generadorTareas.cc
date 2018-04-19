#include "glob.h"
#include "generadorTareas.h"
#include "procesador.h"

generadorTareas::generadorTareas(const string& _name, uint32_t _totalTareas) : process(_name) {
	double lambda = 1.0/0.5;
	
	tiempoArribo = new rngExp("Tiempo de arribo", lambda);
	tiempoArribo->reset();
	
	totalTareas = _totalTareas;
	name        = _name;
	
	registro = new Registro();

	
}


void generadorTareas::inner_body(){
	double tHold;
	
	std::default_random_engine gb_random;
	std::uniform_real_distribution<> uniform(5.33, 10.44);
	
	for(uint32_t tarea=0; tarea < totalTareas; tarea+=1){
		registro->print(this->time(), name, string("Arribo Tarea id:")+string(std::to_string(tarea)) );
	
		procesador->agregarTarea(new Tarea( tarea, uniform(gb_random) ));
		
		if( procesador->idle() && !procesador->estaEsperandoPorCore() ){
			registro->print(this->time(), name, "Activando Procesador");
			procesador->activate();
		}
			
		tHold = tiempoArribo->value();		
		hold(tHold);
	}
	
}

void generadorTareas::asociarProcesador(handle<Procesador> _procesador){
	procesador = _procesador;
}

generadorTareas::~generadorTareas(){
	
}

