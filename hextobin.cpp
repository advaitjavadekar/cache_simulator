
#include "myheader.h"

using namespace std;

string hextobin(string hex_addr,int linelength){
int i;
char bin_addr[64];
	for (i = 2; i < linelength; i++) {

					switch (hex_addr[i]) {
					case '0':
						bin_addr[(i-2)*4] = '0';
						bin_addr[(i-2)*4 + 1] = '0';
						bin_addr[(i-2)*4 + 2] = '0';
						bin_addr[(i-2)*4 + 3] = '0';
						break;

					case '1':
						bin_addr[(i-2)*4] = '0';
						bin_addr[(i-2)*4 + 1] = '0';
						bin_addr[(i-2)*4 + 2] = '0';
						bin_addr[(i-2)*4 + 3] = '1';
						break;

					case '2':
						bin_addr[(i-2)*4] = '0';
						bin_addr[(i-2)*4 + 1] = '0';
						bin_addr[(i-2)*4 + 2] = '1';
						bin_addr[(i-2)*4 + 3] = '0';
						break;

					case '3':
						bin_addr[(i-2)*4] = '0';
						bin_addr[(i-2)*4 + 1] = '0';
						bin_addr[(i-2)*4 + 2] = '1';
						bin_addr[(i-2)*4 + 3] = '1';
						break;

					case '4':
						bin_addr[(i-2)*4] = '0';
						bin_addr[(i-2)*4 + 1] = '1';
						bin_addr[(i-2)*4 + 2] = '0';
						bin_addr[(i-2)*4 + 3] = '0';
						break;

					case '5':
						bin_addr[(i-2)*4] = '0';
						bin_addr[(i-2)*4 + 1] = '1';
						bin_addr[(i-2)*4 + 2] = '0';
						bin_addr[(i-2)*4 + 3] = '1';
						break;


					case '6':
						bin_addr[(i-2)*4] = '0';
						bin_addr[(i-2)*4 + 1] = '1';
						bin_addr[(i-2)*4 + 2] = '1';
						bin_addr[(i-2)*4 + 3] = '0';
						break;

					case '7':
						bin_addr[(i-2)*4] = '0';
						bin_addr[(i-2)*4 + 1] = '1';
						bin_addr[(i-2)*4 + 2] = '1';
						bin_addr[(i-2)*4 + 3] = '1';
						break;

					case '8':
						bin_addr[(i-2)*4] = '1';
						bin_addr[(i-2)*4 + 1] = '0';
						bin_addr[(i-2)*4 + 2] = '0';
						bin_addr[(i-2)*4 + 3] = '0';
						break;

					case '9':
						bin_addr[(i-2)*4] = '1';
						bin_addr[(i-2)*4 + 1] = '0';
						bin_addr[(i-2)*4 + 2] = '0';
						bin_addr[(i-2)*4 + 3] = '1';
						break;

					case 'a':
						bin_addr[(i-2)*4] = '1';
						bin_addr[(i-2)*4 + 1] = '0';
						bin_addr[(i-2)*4 + 2] = '1';
						bin_addr[(i-2)*4 + 3] = '0';
						break;

					case 'b':
						bin_addr[(i-2)*4] = '1';
						bin_addr[(i-2)*4 + 1] = '0';
						bin_addr[(i-2)*4 + 2] = '1';
						bin_addr[(i-2)*4 + 3] = '1';
						break;

					case 'c':
						bin_addr[(i-2)*4] = '1';
						bin_addr[(i-2)*4 + 1] = '1';
						bin_addr[(i-2)*4 + 2] = '0';
						bin_addr[(i-2)*4 + 3] = '0';
						break;

					case 'd':
						bin_addr[(i-2)*4] = '1';
						bin_addr[(i-2)*4 + 1] = '1';
						bin_addr[(i-2)*4 + 2] = '0';
						bin_addr[(i-2)*4 + 3] = '1';
						break;

					case 'e':
						bin_addr[(i-2)*4] = '1';
						bin_addr[(i-2)*4 + 1] = '1';
						bin_addr[(i-2)*4 + 2] = '1';
						bin_addr[(i-2)*4 + 3] = '0';
						break;

					case 'f':
						bin_addr[(i-2)*4] = '1';
						bin_addr[(i-2)*4 + 1] = '1';
						bin_addr[(i-2)*4 + 2] = '1';
						bin_addr[(i-2)*4 + 3] = '1';
						break;

					case '\0':
						break;

					default:
						break;
					}


	}

	return(bin_addr);
}
