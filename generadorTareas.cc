#include "glob.h"
#include "generadorTareas.h"
#include "procesador.h"
#include "tarea.h"

generadorTareas::generadorTareas(const string& _name, uint32_t _totalTareas) : process(_name) {
	double lambda = 1.0/0.5;
	
	tiempoArribo = new rngExp("Tiempo de arribo", lambda);
	tiempoArribo->reset();
	
	totalTareas = _totalTareas;
	name        = _name;
	
	registro = new Registro();

}


void generadorTareas::inner_body(){
	Tarea *tarea;
	
	double tHold;
	
	for(uint32_t tareaID=0; tareaID < totalTareas; tareaID+=1){
		registro->print(this->time(), name, string("Arribo Tarea id:")+string(std::to_string(tareaID)) );
		
		tarea = new Tarea( tareaID);
		//tarea->verDatos();
		
		procesador->agregarTarea(tarea);
		
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

