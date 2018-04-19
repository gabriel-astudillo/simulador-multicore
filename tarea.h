#ifndef _TAREA_H_
#define _TAREA_H_


class Tarea {
private:
	uint32_t id;
	double tServicio;
	
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
	
};



#endif

