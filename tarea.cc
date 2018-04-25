#include "glob.h"
#include "tarea.h"

Tarea::Tarea(uint32_t _id){
	
		id = _id;
		
		std::default_random_engine gb_random(id*10);
		std::uniform_real_distribution<> uniform(5.33, 10.44);
		
		tServicio = uniform(gb_random);
		
		asignarDatos();
	}
	
uint32_t Tarea::getID(){
	return(id);
}

double Tarea::getTservicio(){
	return(tServicio);
}

void Tarea::asignarDatos(){
	for(uint8_t i=0; i < rand()%10; i++){
		agregarDato(65+rand()%10);
	}
}

void Tarea::agregarDato(char dato){
	datos.push_back(dato);
}

void Tarea::verDatos(){
	list<char>::iterator indexDato;
	
	indexDato = datos.begin();
	
	while( indexDato != datos.end() ){
		std::cout << *indexDato << " ";
		indexDato++;
	}
	std::cout << std::endl;
}