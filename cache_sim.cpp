
#include "structures.h"

using namespace std;


unsigned long long int BLOCKSIZE, L1_SIZE, L1_ASSOC, L2_SIZE, L2_ASSOC,address_size;
int L1_no_sets, L1_indexsize, L1_offsetsize, L1_tagsize;
int L2_no_sets, L2_indexsize, L2_offsetsize, L2_tagsize;
int REPL_POLICY;
int INCLUSION_PROPERTY;
char* inclusion_property;
char* replacement_policy;
char* trace_file;
int repl_policy,incl_property;
unsigned long long int read_hits,write_hits,reads,reads2,writes,writes2,read_misses,write_misses;
unsigned long long int L1_reads, L1_read_misses, L1_writes, L1_write_misses;
float L1_miss_rate;  //(L1 read misses + L1 write misses) / (L1 reads + L1 writes)
unsigned long long int L1_writebacks; //dirty evictions from the L1(with an Exclusive L2, clean L1 evictions are also written to L2 but don’t count those here)
unsigned long long int L2_reads;      //L1 read misses + L1 write misses
unsigned long long int L2_read_misses;
unsigned long long int L2_writes; //writebacks from L1, in case of an Inclusive or Non‐inclusive L2 = writebacks from L1 + clean L1 evictions, in case of an Exclusive L2
unsigned long long int L2_write_misses;
float L2_miss_rate;  // (L2 read misses + L2 write misses) / (L2 reads + L2 writes)
unsigned long long int L2_writebacks;
unsigned long long int back_invalidations;
unsigned long long int memory_traffic; //number of blocks transferred to / from memory
unsigned long long int hits,misses,lines;
cache *L1;
cache *L2;
int main(int argc, char* argv[])
	{

		int address_size;
		int  i,x;
		char L1_tagbin[64], L1_indexbin[64], L2_tagbin[64],L2_indexbin[64];
		char hex_addr[64],bin_addr[64];
		string bin_addr_s;
		unsigned long long int L1_tag,L1_index,L2_tag,L2_index;
		string line, binstr;
		stringstream ss;
		unsigned long long int reads=0,writes=0;

//initializers
		L1_reads=0;
		L1_writes=0;
		L1_read_misses=0;
		L1_write_misses=0;
		L1_miss_rate=0;
		L1_writebacks=0;
		L2_reads=0;
		L2_writes=0;
		L2_read_misses=0;
		L2_write_misses=0;
		L2_miss_rate=0;
		L2_writebacks=0;
		memory_traffic=0;
		hits=0;
		misses=0;
		char lru[] = "LRU";
		char fifo[] = "FIFO";
		char inc[]="inclusive";
		char ninc[]="non-inclusive";
		char exc[]="exclusive";

// input arguments

		BLOCKSIZE = atoi(argv[1]);
		L1_SIZE = atoi(argv[2]);
		L1_ASSOC = atoi(argv[3]);
		L2_SIZE = atoi(argv[4]);
		L2_ASSOC = atoi(argv[5]);
		REPL_POLICY = atoi(argv[6]);
		INCLUSION_PROPERTY = atoi(argv[7]);
		trace_file = argv[8];

// Knowing properties of Cache

		L1_no_sets= L1_SIZE/(BLOCKSIZE*L1_ASSOC);
		L1_indexsize = log2(L1_no_sets);
		L1_offsetsize=log2(BLOCKSIZE);
		L1 = new cache(L1_no_sets,L1_ASSOC,1);

		if(L2_SIZE!=0){
			L2_no_sets= L2_SIZE/(BLOCKSIZE*L2_ASSOC);
			L2_offsetsize=log2(BLOCKSIZE);
			L2_indexsize = log2(L2_no_sets);
			L2 = new cache(L2_no_sets,L2_ASSOC,2);
		}
		else{
			L2_no_sets=0;
			L2_offsetsize=0;
			L2_indexsize=0;

		}

		/*cout<<"L1_indexsize="<<L1_indexsize<<endl;
		cout<<"L1_offsetsize="<<L1_offsetsize<<endl;
		cout<<"L2_indexsize="<<L2_indexsize<<endl;
		cout<<"L2_offsetsize="<<L2_offsetsize<<endl;
		 */
		if (REPL_POLICY==0) {			//LRU
					repl_policy = 0;
					replacement_policy=lru;
				}

		else if (REPL_POLICY==1) {		//FIFO
					repl_policy = 1;
					replacement_policy=fifo;
				}
		else
			cout<<"error";

		if (INCLUSION_PROPERTY==0) {	//Non-inclusive
					incl_property = 0;
					inclusion_property=ninc;
				}

		else if (INCLUSION_PROPERTY==1) {	//Inclusive
					incl_property = 1;
					inclusion_property=inc;
				}

		else if (INCLUSION_PROPERTY==2) {	//Exclusive
					incl_property = 2;
					inclusion_property=exc;
				}
		else
			cout<<"error";
// opening input file

		ifstream file(trace_file);

		if (file.is_open())
		{

			while (getline(file, line))
			{

				address_size = ((line.size()-2)*4);
				strncpy(hex_addr, line.c_str(),64);
				//cout<<hex_addr<<endl;
//converting hex to binary
				bin_addr_s=hextobin(hex_addr,line.length());
				strncpy(bin_addr,bin_addr_s.c_str(),48);
				//cout<<bin_addr<<endl;


//Segregating Index and Tag from Address

				L1_tagsize = address_size - L1_indexsize - L1_offsetsize;
				L2_tagsize = address_size - L2_indexsize - L2_offsetsize;

				for(i=0;i<L1_tagsize;i++){
					L1_tagbin[i] = bin_addr[i];
				}
				//cout<<L1_tagbin<<endl;
				L1_tag=bintodec(L1_tagbin);
				//cout<<L1_tag<<endl;

				for(i=0;i<L1_tagsize;i++){
					L2_tagbin[i] = bin_addr[i];
				}
				L2_tag=bintodec(L2_tagbin);

				for(i=L1_tagsize;i<L1_indexsize+L1_tagsize;i++){
					L1_indexbin[i-L1_tagsize] = bin_addr[i];
				}
				//cout<<L1_indexbin<<endl;
				L1_index=bintodec(L1_indexbin);
				//cout<<L1_index<<endl;

				for(i=L2_tagsize;i<L2_indexsize+L2_tagsize;i++){
					L2_indexbin[i-L2_tagsize] = bin_addr[i];
				}
				L2_index=bintodec(L2_indexbin);


// Go for Read or Write
				lines++;
				//cout<<"line"<<lines<<endl;

		if (hex_addr[0] == 'r') {
//read function
					reads++;
					//cout<<"read"<<reads<<endl;
					x=read_function(L1_tag,L1_index,1);

				}

		else if (hex_addr[0] == 'w') {
//write function
				writes++;
				//cout<<"write"<<writes<<endl;
					x=write_function(L1_tag,L1_index,1);
				}
				else
					cout << "error";


			}
				file.close();

		}

		else
			cout << "Failed";

		L1_miss_rate=float(L1_read_misses+L1_write_misses)/float(L1_reads+L1_writes);
		memory_traffic= L1_writebacks+L1_write_misses+L1_read_misses;
		if(L2_SIZE!=0){
			L2_miss_rate=float(L2_read_misses+L2_write_misses)/float(L2_reads+L2_writes);
			memory_traffic=L2_read_misses+L2_write_misses+L2_writebacks;
		}


		delete L1;
		delete L2;





cout << "===== Simulator configuration =====" << endl;
		cout << "BLOCKSIZE:             " << BLOCKSIZE << endl;
		cout << "L1_SIZE:               " << L1_SIZE << endl;
		cout << "L1_ASSOC:              " << L1_ASSOC << endl;
		cout << "L2_SIZE:               " << L2_SIZE << endl;
		cout << "L2_ASSOC:              " << L2_ASSOC << endl;
		cout << "REPLACEMENT POLICY:    " << replacement_policy << endl;
		cout << "INCLUSION PROPERTY:    " << inclusion_property << endl;
		cout << "trace_file:            " << trace_file << endl;
		cout << "===== Simulation results (raw) =====" << endl;
		cout << "a.number of L1 reads:         " << L1_reads << endl;
		cout << "b.number of L1 read misses:   " << L1_read_misses << endl;
		cout << "c.number of L1 writes:        " << L1_writes << endl;
		cout << "d.number of L1 write misses:  " << L1_write_misses << endl;
		cout << "e.L1 miss rate:               " << L1_miss_rate << endl;
		cout << "f.number of L1 writebacks:    " << L1_writebacks << endl;
		cout << "g.number of L2 reads:         " << L2_reads << endl;
		cout << "h.number of L2 read misses:   " << L2_read_misses << endl;
		cout << "i.number of L2 writes:        " << L2_writes<< endl;
		cout << "j.number of L2 write misses:  " << L2_write_misses << endl;
		cout << "k.L2 miss rate:               " << L2_miss_rate << endl;
		cout << "l.number of L2 writebacks:    " << L2_writebacks << endl;
		cout << "m.total memory traffic:       " << memory_traffic << endl;

		return 0;
	}
