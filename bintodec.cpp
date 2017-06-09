
#include <iostream>
#include<string>
#include"math.h"
using namespace std;



unsigned long long int bintodec(string x)
{
	int i;
	unsigned long long int a=0;

	for(i=x.length()-1;i>-1;i--){

		if(x[i]=='1'){
			a=a+pow(2,x.length()-1-i);
		}
	}

	return(a);
}
