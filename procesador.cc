#include "glob.h"
#include "procesador.h"


Procesador::Procesador(const string& _name, uint8_t _totalCores) : process(_name) {
	L2 = 40; /* Cantidad máxima de datos en L2 */
	
	name = _name;
	totalCores = _totalCores;
	esperandoPorCore = false;
	
	registro = new Registro();
	
	asociarCores();
	
}


void Procesador::inner_body(){
	Tarea *tarea;
	
	std::list< handle<coreSim> >::iterator indexCore;
	indexCore = cores.begin();
	while( indexCore != cores.end() ){
		(*indexCore)->activate();
		indexCore++;
	}
	
	while(1){
		
		if( filaTareas.empty() ){
			registro->print(this->time(), name, "Fila vacía, procesador idle");
			esperandoPorCore = false;
			passivate();
		}
		
		tarea = filaTareas.front();
		filaTareas.pop_front();
		
		registro->print(this->time(), name, 
			string("INICIO Asignación tarea id:") + string(std::to_string(tarea->getID())) + string(", tServicio:" ) + string(std::to_string(tarea->getTservicio()))
				);
		
		/* Aquí se debe enviar la tarea a los core que tiene
		   asociado el procesador 
		   Lo anterior significa que el core NO TIENE 
		   una lista de tareas pendientes. Sólo ejecuta
		   una y espera la siguiente
		*/
		/*
			Ciclo de asignación de tareas
		*/
		std::list< handle<coreSim> >::iterator indexCore;
		while(1){
			
			/* 
			   Ciclo de revisión de cores.
			   Revisar cada core. Si no tiene una tarea asignada,
			   se la asigna una.
			*/
			indexCore = cores.begin();
			while( indexCore != cores.end() && tarea != NULL){
				if( ! (*indexCore)->tieneTareaAsignada() ){
					esperandoPorCore = false;
					(*indexCore)->agregarTarea(tarea);	
				
					if( (*indexCore)->idle() ){
						registro->print(this->time(), name, 
							string("FIN Asignación tarea id:") + string(std::to_string(tarea->getID())) + string(" --> ") + (*indexCore)->getName()
							);						
						(*indexCore)->activate();
					}
				
					/* La tarea ya está asignada, hay que 
					   salir del ciclo de revisión de cores 
					*/
					tarea = NULL;
				}
				
				indexCore++;
			} //FIN ciclo de revisión de cores
			
			/*
				Si la tarea no fue asignada (todos los cores tienen una),
			    entonces el procesador debe esperar hasta que uno se 
				libere.
				En caso contrario, debe salir del ciclo de asignación.
			*/
			if( tarea != NULL ){
				esperandoPorCore = true;
				registro->print(this->time(), name, 
					string("Esperando por core para tarea id:") + string(std::to_string(tarea->getID() ))
					);
				passivate();
			}
			else{
				break;
			}
			
		} //FIN Ciclo de asignación de tareas	
		
	}
	
}

void Procesador::agregarTarea(Tarea* tarea){
	filaTareas.push_back(tarea);
}

void Procesador::asociarCores(){
	for(uint8_t i=0; i < totalCores; i++){
		cores.push_back(new coreSim(string("Core") + string(std::to_string(i)) ));
	}
	
	std::list< handle<coreSim> >::iterator indexCore;
	indexCore = cores.begin();
	while( indexCore != cores.end() ){
		(*indexCore)->asociarProcesador(this);
		indexCore++;
	}
}

bool Procesador::filaTareasEstaVacia(){
	return( filaTareas.empty() );
}

bool Procesador::estaEsperandoPorCore(){
	return( esperandoPorCore );
}

Procesador::~Procesador(){
	
}


///////////////////////////


coreSim::coreSim(const string& _name) : process(_name) {
	L1 = 15; /* Cantidad de datos de la memoria L1 */
	
	name = _name;
	
	registro = new Registro();
}


void coreSim::inner_body(){
	
	while(1){
	
		if(tarea == NULL){
			std::cout << std::setw(TIME_WIDTH) << std::fixed <<this->time() << 
				": " << name << ": Sin tarea, idle" << std::endl;
			passivate();
		}
		
		std::cout << std::setw(TIME_WIDTH) << std::fixed << this->time() << 
			": " << name << ": INICIO tarea id:" << tarea->getID() << 
			", tServicio:" << tarea->getTservicio() << std::endl;
		
		/*Simular procesamiento de la tarea*/
		hold( tarea->getTservicio() );
		
		std::cout << std::setw(TIME_WIDTH) << std::fixed << this->time() << 
			": " << name << ": FIN    tarea id:" << tarea->getID() << std::endl;
		
		
		tarea = NULL;
		
		//ACTIVAR EL PROCESADOR SI ESTA IDLE
		//A CAUSA DE ESTAR ESPERANDO UN CORE LIBRE
		if( procesador->idle() && procesador->estaEsperandoPorCore() ){
			std::cout << std::setw(TIME_WIDTH) << std::fixed << this->time() << 
				": " << name << ": Activando Procesador" <<std::endl;
			procesador->activate();
		}
	}
	
}

void coreSim::agregarTarea(Tarea* _tarea){
	tarea = _tarea;
}

void coreSim::asociarProcesador(handle<Procesador> _procesador){
	procesador = _procesador;
}

bool coreSim::tieneTareaAsignada(){
	if(tarea == NULL){
		return(false);
	}
	else{
		return(true);
	}
}

string coreSim::getName(){
	return(name);
}

coreSim::~coreSim(){
	
}


