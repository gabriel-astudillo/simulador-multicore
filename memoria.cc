#include "glob.h"
#include "memoria.h"

Memoria::Memoria(t_memoria _tipo, uint8_t _capacidad){
	tipo      = _tipo;
	capacidad = _capacidad;
	
	datosEnMemoria = new std::string(capacidad, 'x');
	
}

std::string Memoria::verDatos(){
	return (*datosEnMemoria);
}

void Memoria::ponerDato(char dato){
	
	std::size_t datoEncontrado = datosEnMemoria->find_first_of(dato);
	
	if( datoEncontrado == std::string::npos ){
		/* No se encontró el dato 
		*  Se almacena en la posición 'pos'
		*/
		(*datosEnMemoria)[pos%capacidad] = dato;		
	}
}

