/*
 * myheader.h
 *
 *  Created on: 15-Feb-2017
 *      Author: hp
 */

#ifndef MYHEADER_H_
#define MYHEADER_H_
#include<string>
#include<iostream>
#include<fstream>
#include<string>
#include<math.h>
#include<sstream>
#include<cstdlib>
#include "stdio.h"
#include "string.h"
#include <new>


extern unsigned long long int BLOCKSIZE, L1_SIZE, L1_ASSOC, L2_SIZE, L2_ASSOC,address_size;
extern int L1_no_sets, L1_indexsize, L1_offsetsize, L1_tagsize;
extern int L2_no_sets, L2_indexsize, L2_offsetsize, L2_tagsize;
extern int REPL_POLICY;
extern int INCLUSION_PROPERTY;
extern char* trace_file;
extern int repl_policy,incl_property;
extern unsigned long long int read_hits,write_hits,reads,reads2,writes,writes2,read_misses,write_misses;
extern unsigned long long int L1_reads, L1_read_misses, L1_writes, L1_write_misses;
extern float L1_miss_rate;  //(L1 read misses + L1 write misses) / (L1 reads + L1 writes)
extern unsigned long long int L1_writebacks; //dirty evictions from the L1(with an Exclusive L2, clean L1 evictions are also written to L2 but don’t count those here)
extern unsigned long long int L2_reads, L2_read_misses, L2_writes, L2_write_misses;//L1 read misses + L1 write misses
extern float L2_miss_rate;  // (L2 read misses + L2 write misses) / (L2 reads + L2 writes)
extern unsigned long long int L2_writebacks;
extern unsigned long long int back_invalidations;
extern unsigned long long int memory_traffic; //number of blocks transferred to / from memory
extern unsigned long long int hits;
extern unsigned long long int misses,lines;
using namespace std;

string hextobin(string,int);
unsigned long long int bintodec(string);
int read_function(unsigned long long int,unsigned long long int,int);
int write_function(unsigned long long int,unsigned long long int,int);
int update_counter(unsigned long long int,unsigned long long int,int,int,int);
int blocktoevict(unsigned long long int, unsigned long long int,int);
int L2_read_function(unsigned long long int,unsigned long long int,int);
int L2_write_function(unsigned long long int,unsigned long long int,int);
int L2_write_clean_function(unsigned long long int,unsigned long long int,int);
int evict_block(unsigned long long int,unsigned long long int,int);
int check_L2(unsigned long long int,unsigned long long int);


#endif /* MYHEADER_H_ */
