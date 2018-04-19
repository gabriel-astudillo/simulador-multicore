#ifndef GLOBAL_H
#define GLOBAL_H

#include <cppsim.hh>
#include <iostream>
#include <iomanip>
#include <map>
#include <sstream>
#include <set>
#include <vector>
#include <list>
#include <assert.h>
#include <fstream>
#include <string>
#include <limits>
#include <queue>
#include <math.h>
#include <algorithm>
#include <unistd.h>
#include <random>

#define TIME_WIDTH 12
using namespace std;


class Registro {
	
public:
	Registro(){
		
	}
	
	void print(double tiempo, string quien, string mensaje){
		std::cout << std::setw(TIME_WIDTH) << std::fixed << tiempo << 
			": " << quien << ": " << mensaje << std::endl;
	}
	
};

#endif

