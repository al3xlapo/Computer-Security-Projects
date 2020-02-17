#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "blake_ref.h"


typedef struct rainbowTable{
	char* starting_point;
	char* end_point;
}rainbowTable;

typedef struct aux{
	char c;
	int v;
}aux;
