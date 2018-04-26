#include "glob.h"
#include "procesador.h"
#include "tarea.h"


Procesador::Procesador(const string& _name, uint8_t _totalCores) : process(_name), Memoria() {
	size_L2 = 40; /* Cantidad máxima de datos en L2 */
	
	name = _name;
	totalCores = _totalCores;
	esperandoPorCore = false;
	
	registro = new Registro();
	
	Memoria::configuraMemoria(L2, size_L2);
	
	asociarCores();
	
	registro->print(this->time(), name , 
		string("Contenido inicial L2:") + this->verDatos());
	
}


void Procesador::inner_body(){
	Tarea *tarea;
	
	/* Activa los cores asociados al procesador */
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
		coreSim *core;
		
		core = new coreSim(string("Core") + string(std::to_string(i)) );
		core->asociarProcesador(this);
		
		cores.push_back(core);
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


coreSim::coreSim(const string& _name) : process(_name), Memoria() {	
	size_L1 = 15; /* Cantidad máxima de datos en L1 */
	name = _name;
	
	registro = new Registro();
	
	Memoria::configuraMemoria(L1, size_L1);
	
	//registro->print(this->time(), name , 
	//	string("Contenido inicial L1:") + this->verDatos());
}


void coreSim::inner_body(){
	
	
	while(1){
	
		if(tarea == NULL){
			registro->print(this->time(), name , "Sin tarea, idle");
			passivate();
		}
		
		registro->print(this->time(), name , \
			string("INICIO tarea id:") +     \
			string( std::to_string(tarea->getID())) + \
			string(", tServicio:") +  \
			string(std::to_string(tarea->getTservicio()) ));
			
		
		/*==Simular procesamiento de la tarea==*/
		/*
			Cada tarea tiene cierta cantidad de datos que procesar.
				==> Hay que recorrer la lista de datos de la tarea
			Suposición: 
				El tiempo de servicio de la tarea es lo que se
				demora cada dato en procesar
		*/	
		
		while( !tarea->datos.empty() ){
			char datoProcesar;
			
			datoProcesar = tarea->datos.front();
			tarea->datos.pop_front();
			
			tarea->setTInicioServicio(this->time());
			
			registro->print(this->time(), name , \
				string("PROCESAMIENTO tarea id:") + \
				string( std::to_string(tarea->getID())) + \
				string(", Dato:") + \
				datoProcesar \
			);
				
			// Revisar si datoProcesar está en L1
			t_dataStatus dataStatus_L1, dataStatus_L2;
			
			dataStatus_L1 = this->buscarDato(datoProcesar);
			
			if( dataStatus_L1 == DATA_FAIL ){
				/* 'datoProcesar' no está en memoria L1 */
				registro->print(this->time(), name , \
					string("PROCESAMIENTO tarea id:") + \
					string( std::to_string(tarea->getID())) + \
					string(", Dato:") + \
					datoProcesar + \
					string(" DATA_FAIL en L1") \
				);
				
				// Revisar si datoProcesar está en L2
				// Buscar el dato en L2
				dataStatus_L2 = procesador->buscarDato(datoProcesar);
				if( dataStatus_L2 == DATA_FAIL ){
					/* 'datoProcesar' no está en memoria L2 */
					registro->print(this->time(), name , \
						string("PROCESAMIENTO tarea id:") + \
						string( std::to_string(tarea->getID())) + \
						string(", Dato:") + \
						datoProcesar + \
						string(" DATA_FAIL en L2") \
					);
					//Transferir desde RAM hacia L2 (hay costo de transferencia) 
					procesador->ponerDato(datoProcesar);
					hold( TR_RAM_L2 );	
					//Transferir desde L2 a L1 (hay costo de transferencia)		 	
					this->ponerDato(datoProcesar);	
					hold( TR_L2_L1 );			
				}
				else{
					/* 'datoProcesar' sí está en memoria L2 */
					registro->print(this->time(), name , \
						string("PROCESAMIENTO tarea id:") + \
						string( std::to_string(tarea->getID())) + \
						string(", Dato:") + \
						datoProcesar + \
						string(" DATA_OK en L2") \
					);
					//Transferir desde L2 a L1 (hay costo de transferencia)			
					this->ponerDato(datoProcesar);	
					hold( TR_L2_L1 );			
				}									
			}
			
			/* 
			*   'datoProcesar' sí está en memoria L1 
			*   se puede procesar  
			*/
			registro->print(this->time(), name , \
				string("PROCESAMIENTO tarea id:") + \
				string( std::to_string(tarea->getID())) + \
				string(", Dato:") + \
				datoProcesar + \
				string(" DATA_OK en L1") \
			);

			hold( tarea->getTservicio() );	

		}
			
		tarea->setTFinServicio(this->time());
		registro->print(this->time(), name, \
			string("FIN    tarea id:") +  \
			string(std::to_string(tarea->getID()) ) \
		);
		
		
		g_tiempoServicio->update( tarea->getTFinServicio() - tarea->getTInicioServicio() );
		
		
		tarea = NULL;
		
		//ACTIVAR EL PROCESADOR SI ESTA IDLE
		//A CAUSA DE ESTAR ESPERANDO UN CORE LIBRE
		if( procesador->idle() && procesador->estaEsperandoPorCore() ){
			registro->print(this->time(), name, "Activando Procesador");
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


