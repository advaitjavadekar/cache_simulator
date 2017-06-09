
#include "structures.h"

int read_function(unsigned long long int tagg,unsigned long long int index,int level){
	L1_reads++;
	unsigned long long int i=0,x=0,r=0,evicted_block,ret_value,y,incbit,dirtybit;
	unsigned long long int dirty_tag,dirty_index,dirty_address,miss_tag,miss_index,miss_address;
	unsigned long long int L2_tag,L2_index;
	//cout<<"over here now";
	//check for hits in this loop
	//if hit
	for(i=0;i<L1_ASSOC;i++){
			if(tagg==L1->s[index]->b[i]->tag && L1->s[index]->b[i]->valid==1){
				hits++;
				//cout<<"hit"<<hits<<endl;
				ret_value= 1;
				x=update_counter(tagg,index,i,level,1);
				return ret_value;

			}
	}
	//if it comes out of this loop it is a miss

	L1_read_misses++;
	//cout<<"miss"<<misses<<endl;
	misses++;
	//L1 miss check for the block in L2
	miss_tag=tagg<<(L1_indexsize+L1_offsetsize);
	miss_index= index <<L1_offsetsize;
	miss_address= miss_tag+miss_index;

	//now check for valid=0 to find empty blocks and fill them
	for(i=0;i<L1_ASSOC;i++){
		if(L1->s[index]->b[i]->valid==0){    //basically valid=0 means you can write in that block/block is empty
			L1->s[index]->b[i]->tag=tagg;    //tag written in block
			L1->s[index]->b[i]->valid=1;     //occupied block
			x=update_counter(tagg,index,i,level,0);
			ret_value=0;					//if we find a tag write in it then we can return 0 indicating miss
			//return ret_value;
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
					/*else
					L1->s[index]->b[i]->dirty=0;*/
			}
			//if dirty in L2 while evicting get it back and go to L1 again paste it with dirty bit
			break;
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
			dirty_tag=(L1->s[index]->b[evicted_block]->tag) << (L1_indexsize+L1_offsetsize);
			dirty_index= index << L1_offsetsize;
			dirty_address= dirty_tag + dirty_index;
			if(L2_SIZE!=0){
				L2_tag=dirty_address>>(L2_offsetsize+L2_indexsize);
				L2_index=(dirty_address-(L2_tag<<(L2_offsetsize+L2_indexsize)))>>L2_offsetsize;
				//now writeback to L2
				y=L2_write_function(L2_tag,L2_index,2);
			}
		}

		else if(L1->s[index]->b[evicted_block]->dirty==0 && incl_property==2){
			//clean evictions no L1writebacks++
			//get address for writing to L2 here
			dirty_tag=(L1->s[index]->b[evicted_block]->tag) << (L1_indexsize+L1_offsetsize);
			dirty_index= index << L1_offsetsize;
			dirty_address= dirty_tag + dirty_index;
			if(L2_SIZE!=0){
				L2_tag=dirty_address>>(L2_offsetsize+L2_indexsize);
				L2_index=(dirty_address-(L2_tag<<(L2_offsetsize+L2_indexsize)))>>L2_offsetsize;
				//now writeback to L2 with new function
				y=L2_write_clean_function(L2_tag,L2_index,2);
			}
		}

		//now we know which block is to be evicted if it is valid but not dirty  just replace the tags
		L1->s[index]->b[evicted_block]->tag=tagg;
		L1->s[index]->b[evicted_block]->valid=1;
		L1->s[index]->b[evicted_block]->dirty=0;
		//update counter for LRU
		x=update_counter(tagg,index,evicted_block,level,0);
		///if it is dirty then writeback to L2 and then set dirty=0 as it is a read
		//again since it is a miss here
		ret_value=0;
//*******EXCLU
		//find this block in L2 and remove it give dirty bit to L1 block else do nothing
		if(L2_SIZE!=0 && incl_property==2){
			L2_tag=miss_address>>(L2_offsetsize+L2_indexsize);
			L2_index=(miss_address-(L2_tag<<(L2_offsetsize+L2_indexsize)))>>L2_offsetsize;
			incbit=check_L2(L2_tag,L2_index);
			if(incbit==1){
				dirtybit=evict_block(L2_tag,L2_index,2);
				if(dirtybit==1)
					L1->s[index]->b[evicted_block]->dirty=1;
			}

		}
	}
	//it will return 0 here then
	if(L2_SIZE!=0 && incl_property!=2){
				L2_tag=miss_address>>(L2_indexsize+L2_offsetsize);
				L2_index=(miss_address-(L2_tag<<(L2_indexsize+L2_offsetsize)))>>L2_offsetsize;
				y=L2_read_function(L2_tag,L2_index,2);
			}
	return ret_value;
}


