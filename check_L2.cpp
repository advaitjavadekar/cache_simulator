
#include "structures.h"

int check_L2(unsigned long long int tagg,unsigned long long int index){
	int incbit,i;
	if(incl_property==2){
		L2_reads++;
	}
	for(i=0;i<L2_ASSOC;i++){
		if(tagg==L2->s[index]->b[i]->tag && L2->s[index]->b[i]->valid==1){
			return 1;
		}
	}
	if(incl_property==2){
		L2_read_misses++;
	}
	return 0;
}
