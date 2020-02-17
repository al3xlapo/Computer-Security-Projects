#include "rainbow.h"



long bitXor(long x, long y){
    long a, b, z;
	a = x & y;
    b = ~x & ~y;
    z = ~a & ~b;
    return z;
}     


char* R(long j, char* h, auxiliary* aux){ //reduction function, with parameter j to emulate R1, R2, ... , Rk
	
	long gn = 0, gc = 0, k = 0, i = 0, shift; //gn is the current group I'm creating, gc is the current position in the group string
	char* p = malloc(7*sizeof(char));
	h[strlen(h)] = '\0'; //terminate hash with null
	shift = strlen(h) / 3; //when gc reaches shift, it proceeds to the next group
	char** group = malloc(6 * sizeof(char*));
	for(i=0; i<6; i++)
		group[i] = malloc((shift+1) * sizeof(char)); //group_sum[i] will hold the sum of the ASCII codes of the characters in that group
	char* group_sum = calloc(6, sizeof(long));
	
	while(h[k] != NULL){ //create/form groups and their sums
		if(gc == 6){
			gc = 0;
			gn++;
		}
		group_sum[gn] += h[k];
		group[gn][gc] = h[k];	
		gc++;
		k++;
	}
	for(i=0; i<6; i++){
		//p[i] = ((group_sum[i]+j) XOR group[i]) XOR j) % 64;
	}
	
	
	
	
	
}




char* H(char* p){ //hash function
	//print "hashed p"
	char* h = malloc(sizeof(p));
	strcpy(h, p) ;
	return h;
}
	
	
char* rainbowAttack(rainbowTable rt, char* hash, long chain_num, long chain_len, auxiliary* aux, BLAKE blake){ //using the rainbow table rt, find the password corresponding to hash given
	printf("performing rainbow attack\n");
	long i, j;
	char* p = malloc(7*sizeof(char));
	char* h = malloc(65*sizeof(char));
	char* pc = malloc(7*sizeof(char));
	char* hc = malloc(65*sizeof(char));
	strcpy(h, hash);
	for(i=0; i<chain_num; i++){ //check to see if the hash is an end_point, and if so return the start_point	
		if (!strcmp(h, rt[i].end_point)) //found the hash, so get the corresponding key and recreate the chain until you find the hash again
			return rt[i].starting_point;
	}
	//if it wasn't in the table, start creating the chain. The same testing will be done after each chain iteration		
	//the chain I'm creating ,starting from the given hash, will start from Rk and go down to R1 as it searches the rainbow table for a match
	for(j=0; j<chain_len; j++){ //until the end of the chain, reduce h and rehash it, then check if it's in the table
		p = R(chain_len - (j+1), h, aux); 
		h = blake.BLAKE_func(256, p, 48); //hash with blake
		for(i=0; i<chain_num; i++){ //test if current h is in the table
			if (!strcmp(h, rt[i].end_point)){
				strcpy(pc, rt[i].starting_point]);
				for(k=0; k<chain_len; k++){ //if it is, recreate the chain until you find h again or until you reach chain_len, starting from key
					hc = blake.BLAKE_func(256, p, 48); //hash the p password
					if(!strcmp(hc, h)) //you found wanted hash, so return the password that produced it
						return pc;
					pc = R(j, h, aux); //reduce back to password
				}
			}
		}
	}
	return NULL;
}




	

void createRainbowTable(rainbowTable* rt, long chain_num, long chain_len, BLAKE blake){

	printf("creating rainbow table\n");
	char* starting_point = malloc(7*sizeof(char));
	char* end_point = malloc(65*sizeof(char));
	char* p = malloc(7*sizeof(char));
	char* h = malloc(65*sizeof(char));
	long i, j, k;
	for(i=0; i<chain_num; i++){ //create a starting point for each rt row, using a random string generator
		//starting_point = ''.join([random.choice(string.ascii_letters + string.digits + '!@') for n in xrange(6)])
		strcpy(p, starting_point);
		for(j=0; j<chain_len; j++){ //perform (hash function, redution function) "chain_len"" times to create each chain
			h = blake.BLAKE_func(256, p, 48); //hash the p password
			p = R(j, h); //reduce back to password
		}
		strcpy(end_point, h); //the end_point to be stored in the table is the last hash of the chain, final p is omitted
		strcpy(rt[i].starting_point, p); //store the chain
		strcpy(rt[i].end_point, end_point);
	}
	for(i=0; i<chain_num; i++){
		printf("%s :: %s\n", rt[i].starting_point, rt[i].end_point);
	}
}
	

	

int main(int argc, char* argv){


	if (argc < 2){
		printf("Usage %s <hash>",argv[0]);
		return 1;
	}
	int i;
	long chain_len = 10;
	long chain_num = 10;
	char* password = malloc(7 * sizeof(char));
	char* symbols = malloc(65 * sizeof(char)); //symbols contains all possible symbols of a password
	strcpy(symbols, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@");
	rainbowTable* rt = malloc(chain_num * sizeof(rainbowTable)); //empty rainbow table
	blake = BLAKE(256);
	
	//chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@"
	//the table will have all the possible characters of the password as keys and numbers I'll assign them as values
	//e.g. {'a' : 1, 'b' : 2 .... 'm' @ : 64}, it will be used to generate the passes through the R function
	auxiliary* aux = malloc(64 * sizeof(aux));
	for(i=0; i<64; i++){
		aux[i].c = symbols[i];
		aux[i].v = i;
	}
	createRainbowTable(rt, chain_num, chain_len, blake);
	password = rainbowAttack(rt, argv[1], chain_num, chain_len, aux, blake); //execute a rainbow attack using the rainbow table created on the first argument of the script(the hash)
	if(password == NULL)
		printf("Couldn't find the password\n");
	else
		printf("Password is %s\n", password);
	return 0;
}
	


