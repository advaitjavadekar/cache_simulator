
#include "structures.h"

int L2_write_function(unsigned long long int tagg,unsigned long long int index,int level){
	L2_writes++;
	int i=0,x=0,temp=0,r=0,max=0,evicted_block,ret_value,count=0,evicted_dirty,y,evblock;
	unsigned long long int L1_evict_tag,L1_evict_index,evict_address,L2_evict_tag,L2_evict_index;
	//cout<<"here as  well"<< count;
	count++;

	for(i=0;i<L2_ASSOC;i++){
			if(tagg==L2->s[index]->b[i]->tag && L2->s[index]->b[i]->valid==1){
				hits++;
				//cout<<"hit"<<hits<<endl;
				//write hit so dirty =1
				L2->s[index]->b[i]->dirty=1;
				ret_value= 1;
				x=update_counter(tagg,index,i,level,1);
				return ret_value;

			}
	}
	//if it comes out of this loop it is a miss

	L2_write_misses++;
	//cout << "Line for miss is: " << lines << endl;
	//now check for valid=0 to find empty blocks and fill them
	for(i=0;i<L2_ASSOC;i++){
		if(L2->s[index]->b[i]->valid==0){    //basically valid=0 means you can write in that block/block is empty
			L2->s[index]->b[i]->tag=tagg;    //tag written in block
			L2->s[index]->b[i]->valid=1;     //occupied block
			L2->s[index]->b[i]->dirty=1;	// it is a write so dirty block now
			x=update_counter(tagg,index,i,level,0);
			ret_value=0;					//if we find a tag write in it then we can return 0 indicating miss
			return ret_value;
		}
		else r++;
	}
	//it comes out of this loop then all blocks are full for this set

	//set is full if 'r' = associativity and it will have valid and dirty blocks
		if(r==L2_ASSOC){
			//cout<<"set is full"<<endl; 					//set is full
			evicted_block=blocktoevict(tagg,index,level); //find a block to evict highest LRU/FIFO
			//cout<<"evicted_block="<<evicted_block<<endl;

			//for inclusive only
			//we know the block to be evicted, evict it from L1 now
			if(incl_property==1){
				//get the L1 address for evicting from L1
				L2_evict_tag=(L2->s[index]->b[evicted_block]->tag) << (L2_indexsize+L2_offsetsize);
				L2_evict_index= index << L2_offsetsize;
				evict_address= L2_evict_tag + L2_evict_index;
				L1_evict_tag=evict_address>>(L1_offsetsize+L1_indexsize);
				L1_evict_index=(evict_address-(L1_evict_tag<<(L1_offsetsize+L1_indexsize)))>>L1_offsetsize;
				//now evict block from L1
				evblock=evict_block(L1_evict_tag,L1_evict_index,1);
			}

			if(L2->s[index]->b[evicted_block]->dirty==1){
				L2_writebacks++;

			}
			//now we know which block is to be evicted if it is valid but not dirty  just replace the tags
			L2->s[index]->b[evicted_block]->tag=tagg;
			//update counter for LRU
			x=update_counter(tagg,index,evicted_block,level,0);
			///if it is dirty then writeback to main memory and then set dirty=0 as it is a read

			//again since it is a miss here and a write was done we set dirty=1
			L2->s[index]->b[evicted_block]->dirty=1;
			ret_value=0;
		}
//it will return 0 here then
return ret_value;
	}
