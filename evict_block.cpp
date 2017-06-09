
#include "structures.h"

int evict_block(unsigned long long int tagg,unsigned long long int index,int level){
	int i,dirty;
	if(level==1){
		for(i=0;i<L1_ASSOC;i++){
			if(L1->s[index]->b[i]->tag==tagg){
				L1->s[index]->b[i]->valid=0;
				dirty=L1->s[index]->b[i]->dirty;
				//reset dirty bit
				L1->s[index]->b[i]->lru=0;
				L1->s[index]->b[i]->fifo=0;
				L1->s[index]->b[i]->dirty=0;
				if(dirty==1){
					L1_writebacks++;
					L2_writebacks++;
					return 1;
				}
				return 0;
			}
		}
	}

	else if(level==2){
		for(i=0;i<L2_ASSOC;i++){
			if(L2->s[index]->b[i]->tag==tagg){
				L2->s[index]->b[i]->valid=0;
				dirty=L2->s[index]->b[i]->dirty;
				//reset dirty bit
				L2->s[index]->b[i]->dirty=0;
				if(dirty==1){

					return 1;

				}
				return 0;

			}
		}

	}
	return 0;
}
