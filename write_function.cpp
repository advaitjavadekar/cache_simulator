
#include "structures.h"

int write_function(unsigned long long int tagg,unsigned long long int index,int level){
	L1_writes++;
	int i=0,x=0,r=0,y,evicted_block,ret_value,incbit,dirtybit;
	unsigned long long int dirty_tag,dirty_address,dirty_index,miss_tag,miss_address,miss_index;
	unsigned long long int L2_tag,L2_index;

	for(i=0;i<L1_ASSOC;i++){
			if(tagg==L1->s[index]->b[i]->tag && L1->s[index]->b[i]->valid==1){
				hits++;
				//cout<<"hit"<<hits<<endl;
				//write hit so dirty =1
				L1->s[index]->b[i]->dirty=1;
				ret_value= 1;
				x=update_counter(tagg,index,i,level,1);
				return ret_value;

			}
	}
	//if it comes out of this loop it is a miss
	miss_tag=tagg<<(L1_indexsize+L1_offsetsize);
	miss_index= index <<L1_offsetsize;
	miss_address= miss_tag+miss_index;

	L1_write_misses++;
	//check for block in L2
	//convert to l2 tag and index
	//for inclusive check for block in L2 here to ensure inclusion

	//now check for valid=0 in L1 to find empty blocks and fill them
	for(i=0;i<L1_ASSOC;i++){
		if(L1->s[index]->b[i]->valid==0){    //basically valid=0 means you can write in that block/block is empty
			L1->s[index]->b[i]->tag=tagg;    //tag written in block
			L1->s[index]->b[i]->valid=1;     //occupied block
			L1->s[index]->b[i]->dirty=1;	// it is a write so dirty block now
			x=update_counter(tagg,index,i,level,0);
			ret_value=0;
			//****************for EXCLUSIVE
						//go to L2 find same block and if present remove
			if(L2_SIZE!=0 && incl_property==2){
				L2_tag=miss_address>>(L2_offsetsize+L2_indexsize);
				L2_index=(miss_address-(L2_tag<<(L2_offsetsize+L2_indexsize)))>>L2_offsetsize;
				incbit=check_L2(L2_tag,L2_index);
				if(incbit==1){
					dirtybit=evict_block(L2_tag,L2_index,2);
					if(dirtybit==1)
						L1->s[index]->b[i]->dirty=1;
				}
			}
						//if dirty in L2 while evicting get it back and go to L1 again paste it with dirty bit

			break;//if we find a tag write in it then we can return 0 indicating miss
			//return ret_value;
		}
		else r++;
	}
	//it comes out of this loop then all blocks are full for this set

	//set is full if 'r' = associativity and it will have valid and dirty blocks
	if(r==L1_ASSOC){
			//cout<<"set is full"<<endl; 					//set is full
			evicted_block=blocktoevict(tagg,index,level); //find a block to evict highest LRU/FIFO
			//cout<<"evicted_block="<<evicted_block<<endl;
			if(L1->s[index]->b[evicted_block]->dirty==1){
				L1_writebacks++;
				//get address for writing to L2 here
				dirty_tag=(L1->s[index]->b[evicted_block]->tag << (L1_indexsize+L1_offsetsize));
				dirty_index= index << L1_offsetsize;
				dirty_address= dirty_tag + dirty_index;
				if(L2_SIZE!=0){
				L2_tag=dirty_address>>(L2_offsetsize+L2_indexsize);
				L2_index=(dirty_address-(L2_tag<<(L2_offsetsize+L2_indexsize)))>>L2_offsetsize;
				//now writeback to L2
				y=L2_write_function(L2_tag,L2_index,2);
				}
			}
			//otherwise if exclusive even when not dirty we evict to L2
			else if(L1->s[index]->b[evicted_block]->dirty==0 && incl_property==2){
				//get address for writing to L2 here
				dirty_tag=(L1->s[index]->b[evicted_block]->tag) << (L1_indexsize+L1_offsetsize);
				dirty_index= index << L1_offsetsize;
				dirty_address= dirty_tag + dirty_index;
				if(L2_SIZE!=0){
					L2_tag=dirty_address>>(L2_offsetsize+L2_indexsize);
					L2_index=(dirty_address-(L2_tag<<(L2_offsetsize+L2_indexsize)))>>L2_offsetsize;
					//now writeback to L2
					y=L2_write_clean_function(L2_tag,L2_index,2);
				}
			}
			//now we know which block is to be evicted if it is valid but not dirty  just replace the tags
			L1->s[index]->b[evicted_block]->tag=tagg;
			L1->s[index]->b[evicted_block]->valid=1;
			//update counter for LRU
			x=update_counter(tagg,index,evicted_block,level,0);
			///if it is dirty then writeback to L2 and then set dirty=0 as it is a read

			//again since it is a miss here and a write was done we set dirty=1
			L1->s[index]->b[evicted_block]->dirty=1;
			//****************for EXCLUSIVE
						//go to L2 find same block and if present remove
			if(L2_SIZE!=0 && incl_property==2){
				L2_tag=miss_address>>(L2_offsetsize+L2_indexsize);
				L2_index=(miss_address-(L2_tag<<(L2_offsetsize+L2_indexsize)))>>L2_offsetsize;
				incbit=check_L2(L2_tag,L2_index);
				if(incbit==1){
					dirtybit=evict_block(L2_tag,L2_index,2);
					if(dirtybit==1)
						L1->s[index]->b[evicted_block]->dirty=1;
				}
				//if dirty in L2 while evicting get it back and go to L1 again paste it with dirty bit
			}
		}
	if(L2_SIZE!=0 && incl_property!=2){
			L2_tag=miss_address>>(L2_indexsize+L2_offsetsize);
			L2_index=(miss_address-(L2_tag<<(L2_indexsize+L2_offsetsize)))>>L2_offsetsize;
			y=L2_read_function(L2_tag,L2_index,2);
		}

		ret_value=0;

	//it will return 0 here then
return ret_value;
	}
