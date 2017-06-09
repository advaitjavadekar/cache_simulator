/*
 * structures.h
 *
 *  Created on: 18-Feb-2017
 *      Author: hp
 */

#ifndef STRUCTURES_H_
#define STRUCTURES_H_
#include "myheader.h"

using namespace std;


class block{
public:
	unsigned long long int tag;
	int valid,dirty;
	int lru,fifo;
	block(){
		valid=0;
		dirty=0;
		tag=0;
		lru=0;
		fifo=0;
	}


};

class set{
public:

	int no_blocks;
	block **b;
	set(int assoc) {
		no_blocks=assoc;
		b =new block*[no_blocks];
		for(int i=0;i<no_blocks;i++){
			b[i] = new block();
		}
	}

	~set() {
		for(int i=0;i<no_blocks;i++){
			delete b[i];
		}
	delete b;
	}

};

class cache {
public:
		int no_sets;
		set** s;
		cache(int no_sets,int assoc,int level){
			//this->assoc=assoc;
			this->no_sets = no_sets;
			s=new set*[no_sets];
			for(int i=0;i<no_sets;i++){
				s[i]=new set(assoc);
			}


		}
//declare read and write function
		~cache(){
			for(int i=0;i<no_sets;i++){
				delete s[i];
			}
			delete s;
		}
};
extern cache *L1;
extern cache *L2;


#endif /* STRUCTURES_H_ */
