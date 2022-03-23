#include "header.h"


int prand(int seed){ /* from k&r */
	seed = seed * 1103515245 + 12345;
	return (unsigned int) (seed/65536) % HASH_MAX;
}

unsigned int hash(char *s){
	if(*s == '\0')
		return 1;
	else
		return (prand(*s)+ hash(s+1)) % HASH_MAX;
}

Map newmap(){
	Map m = {NULL};
	return m;
}


int mapget(Map *mp, char *key){
	ListNode *ln = mp->nodes[hash(key)];    
	while(ln != NULL){
		if(strcmp(key, ln->key) == 0)    
		return ln->value;
		ln = ln->next;
	}
    return 0;
}

void mapset(Map *mp, char *key, int value){
	unsigned int keyhash = hash(key);
	ListNode *ln = mp->nodes[keyhash];    
	while(ln != NULL){
		if(strcmp(key, ln->key) == 0){
		    /* found matching node */
			ln->value = value;        
			return;    
		}
		ln = ln->next;
	}
	/* no matching node found */
	char *newkey = malloc(strlen(key));/* error check */
	strcpy(newkey, key);/* copy key to newkey */

	ListNode *newnodep = malloc(sizeof(ListNode));
	newnodep->key = newkey;
	newnodep->value = value;
	newnodep->next = mp->nodes[keyhash];

	mp->nodes[keyhash] = newnodep;
}
