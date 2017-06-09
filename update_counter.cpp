
#include "structures.h"

int update_counter(unsigned long long int tagg,unsigned long long int index,int x,int level,int hit){
	int i,value;
	if(level==1){
		if(repl_policy==0){
			if(hit==1){
				value=L1->s[index]->b[x]->lru;
				for(i=0;i<L1_ASSOC;i++){
					if(value>(L1->s[index]->b[i]->lru)){
						L1->s[index]->b[i]->lru++;
					}
				}
				L1->s[index]->b[x]->lru=0;

			}
			else{
			for(i=0;i<L1_ASSOC;i++){
				L1->s[index]->b[i]->lru++;
				}
				L1->s[index]->b[x]->lru=0;
		}

	}
		else if(repl_policy==1){
			if(hit==1){
				//do nothing on hits

					}
			else{
					for(i=0;i<L1_ASSOC;i++){
						L1->s[index]->b[i]->fifo++;
						}
						L1->s[index]->b[x]->fifo=0;
				}

	}
	}
	else if(level==2){
		if(repl_policy==0){
			if(hit==1){
				value=L2->s[index]->b[x]->lru;
				for(i=0;i<L2_ASSOC;i++){
					if(value>(L2->s[index]->b[i]->lru)){
						L2->s[index]->b[i]->lru++;
					}
				}
				L2->s[index]->b[x]->lru=0;

			}
			else{
				for(i=0;i<L2_ASSOC;i++){
					L2->s[index]->b[i]->lru++;
					}
					L2->s[index]->b[x]->lru=0;
			}

		}
		else
		{
			if(hit==1){
				//do nothing on hits
					}
			else{
						for(i=0;i<L2_ASSOC;i++){
							L2->s[index]->b[i]->fifo++;
							}
							L2->s[index]->b[x]->fifo=0;
					}

		}

	}
	return 1;

}

