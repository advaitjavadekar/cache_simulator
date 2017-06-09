
#include "structures.h"

int blocktoevict(unsigned long long int tagg, unsigned long long int index,int level){
	int i,temp=0,max=0;
	if(level==1){
		if(repl_policy==0){
			for(i=0;i<L1_ASSOC;i++){
				if((L1->s[index]->b[i]->lru)>temp){
					temp=L1->s[index]->b[i]->lru;
				}
			}
			for(i=0;i<L1_ASSOC;i++){
				if(temp==L1->s[index]->b[i]->lru)
					max=i;

			}
		}
		else if(repl_policy==1){
			for(i=0;i<L1_ASSOC;i++){
				if((L1->s[index]->b[i]->fifo)>temp){
					temp=L1->s[index]->b[i]->fifo;
				}
			}
			for(i=0;i<L1_ASSOC;i++){
				if(temp==L1->s[index]->b[i]->fifo)
					max=i;

				}

		}
		else max=0;
	}

	else if (level==2){
		if(repl_policy==0){
			for(i=0;i<L2_ASSOC;i++){
				if((L2->s[index]->b[i]->lru)>temp){
					temp=L2->s[index]->b[i]->lru;
				}
			}
			for(i=0;i<L2_ASSOC;i++){
				if(temp==L2->s[index]->b[i]->lru)
					max=i;

			}
		}
		else if(repl_policy==1){
			for(i=0;i<L2_ASSOC;i++){
				if((L2->s[index]->b[i]->fifo)>temp){
					temp=L2->s[index]->b[i]->fifo;
				}
			}
			for(i=0;i<L2_ASSOC;i++){
				if(temp==L2->s[index]->b[i]->fifo)
					max=i;

				}

		}

	}
	return max;

}
