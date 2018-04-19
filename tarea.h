#ifndef _TAREA_H_
#define _TAREA_H_


class Tarea {
private:
	uint32_t id;
	double tServicio;
	
	list<char> datos;
	
protected:
	
	
public:
	Tarea(uint32_t _id, float _tServicio){
		id = _id;
		tServicio = _tServicio;
	}
	
	uint32_t getID(){
		return(id);
	}
	
	double getTservicio(){
		return(tServicio);
	}
	
	void agregarDato(char dato){
		datos.push_back(dato);
	}
	
	void verDatos(){
		list<char>::iterator indexDato;
		
		indexDato = datos.begin();
		
		while( indexDato != datos.end() ){
			std::cout << *indexDato << " ";
			indexDato++;
		}
		std::cout << std::endl;
	}
	
};



#endif

