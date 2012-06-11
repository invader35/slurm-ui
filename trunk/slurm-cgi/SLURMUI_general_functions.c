#include "slurm.h"
#include "json/json.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "SLURMUI.h"

extern int HASH_FULL;
Partition_Node* node_job_hash[HASH_SIZE];

int _atoi(const char* string){
	
	int i = 0;
	while(*string ){
		i = (i<<3)+(i<<1)+(*string - '0');
		string++;
	}
	return(i);

}

Linked_List_Node* node_job_get(char* key){

	int i = 0;
	long value = 0;
 
	for( ; key[i] != '\0' ; i++){

		value += key[i];

	}

	//requested key not found
	if(node_job_hash[value%HASH_SIZE] == NULL) return NULL;
	
	//requested key found
	else if( strcmp(key,node_job_hash[value%HASH_SIZE]-> key) == 0)
		 return (node_job_hash[value%HASH_SIZE]->value);
	//keep searching the key	
	else{

		i=1;

		while(i<HASH_SIZE){

			value += i;	
			if(node_job_hash[value%HASH_SIZE]==NULL) return NULL;
			else if(strcmp(key,node_job_hash[value%HASH_SIZE]->key) == 0)
				return (node_job_hash[value%HASH_SIZE]->value);
			else
				i++;	

		}
	
		if(i== HASH_SIZE)
			HASH_FULL = 1;
		
	}

	return NULL;

}
	

void append_job(Linked_List_Node** llist, job_info_t* job) {
	
	Linked_List_Node* tmp = *llist; 
	
	//the linked list is empty
	if((*llist)==NULL){
		
		(*llist) = (Linked_List_Node*)malloc(sizeof(Linked_List_Node));
  		(*llist)->value_job = (*job);
  		(*llist)->next = NULL;
		
	}	
	else{
		
		while( tmp->next != NULL)
	  		tmp = tmp->next;

		tmp->next = (Linked_List_Node*)malloc(sizeof(Linked_List_Node));
	  	tmp->next->value_job = (*job);
	  	tmp->next->next = NULL;
	
	}

}



void node_job_put(char* key, job_info_t job){

	int i = 0;	
	long value = 0;

	for( ; key[i] != '\0' ; i++){

		value += key[i];

	}
	
	//an empty cell
	if(node_job_hash[value%HASH_SIZE] == NULL){
		
		node_job_hash[value%HASH_SIZE] = malloc(sizeof(Partition_Node));
		node_job_hash[value%HASH_SIZE]-> key = key;	
		node_job_hash[value%HASH_SIZE]-> value = NULL;	
		append_job(&(node_job_hash[value%HASH_SIZE]->value),&job);


	}
	else if(strcmp(key , node_job_hash[value%HASH_SIZE]-> key) == 0){
	
		append_job(&(node_job_hash[value%HASH_SIZE]->value),&job);	

	}
	//a different partiton occupies cell --> run confliction resolution
	else{	
		i=1;

		while(i<HASH_SIZE){

			value += i;	
			
			if(node_job_hash[value%HASH_SIZE] == NULL){
			
				node_job_hash[value%HASH_SIZE] = malloc(sizeof(Partition_Node));
				node_job_hash[value%HASH_SIZE]-> key = key;
				node_job_hash[value%HASH_SIZE]-> value = NULL;	
				append_job(&(node_job_hash[value%HASH_SIZE]->value),&job);
				break;

			}
			else if(strcmp(key , node_job_hash[value%HASH_SIZE]-> key) == 0){
				
				append_job(&(node_job_hash[value%HASH_SIZE]->value),&job);
				break;		

			}
			else
				i++;

		}

		if(i==HASH_SIZE)
			HASH_FULL = 1;
		
	}

}

int getCurrentTime(char* input){
       
	char *ptr;
 	time_t timer;
 	char month[3];
	char result[10];
	int result_int = 0;
	timer = time(NULL);
 
 	month[0] = input[5];
 	month[1] = input[6];
 	month[2] = input[7]; 
	month[3] = '\0';
 
	result[0] = input[21];
 	result[1] = input[22];
	result[2] = input[23];
 	result[3] = input[24];
 	
	if(input[9] == ' ') result[6] = '0';
	else 		    result[6] = input[9];
	result[7] = input[10];
	result[8] = '\0';

 	if(strcmp (month,"Jan") == 0){
 
	 	result[4] = '0';
 		result[5] = '1';
 		
	}
 	else if(strcmp (month,"Feb") == 0){
	 	result[4] = '0';
 		result[5] = '2';
	}
 	else if(strcmp (month,"Mar") == 0){
	 	result[4] = '0';
 		result[5] = '3';
	}
 	else if(strcmp (month,"Apr") == 0){
	 	result[4] = '0';
 		result[5] = '4';
	}
 	else if(strcmp (month,"May") == 0){
	 	result[4] = '0';
 		result[5] = '5';
	}
 	else if(strcmp (month,"Jun") == 0){
	 	result[4] = '0';
 		result[5] = '6';
	}
 	else if(strcmp (month,"Jul") == 0){
	 	result[4] = '0';
 		result[5] = '7';
	}
 	else if(strcmp (month,"Aug") == 0){
	 	result[4] = '0';
 		result[5] = '8';
	}
 	else if(strcmp (month,"Sep") == 0){
	 	result[4] = '0';
 		result[5] = '9';
	}
 	else if(strcmp (month,"Oct") == 0){
	 	result[4] = '1';
 		result[5] = '0';
	}
 	else if(strcmp (month,"Nov") == 0){
	 	result[4] = '1';
 		result[5] = '1';
	}
 	else if(strcmp (month,"Dec") == 0){
	 	result[4] = '1';
 		result[5] = '2';
	}


	return _atoi(result); 
	

}

int getCurrentTime2(char* input){


 	char month[3];
	char result[10];
	
 
 	month[0] = input[4];
 	month[1] = input[5];
 	month[2] = input[6]; 
	month[3] = '\0';
 
	result[0] = input[11];
 	result[1] = input[12];
	result[2] = input[13];
 	result[3] = input[14];
 		
	result[6] = input[8];
 	result[7] = input[9];
	result[8] = '\0';
 


 	if(strcmp (month,"Jan") == 0){
 
	 	result[4] = '0';
 		result[5] = '1';
 		
	}
 	else if(strcmp (month,"Feb") == 0){
	 	result[4] = '0';
 		result[5] = '2';
	}
 	else if(strcmp (month,"Mar") == 0){
	 	result[4] = '0';
 		result[5] = '3';
	}
 	else if(strcmp (month,"Apr") == 0){
	 	result[4] = '0';
 		result[5] = '4';
	}
 	else if(strcmp (month,"May") == 0){
	 	result[4] = '0';
 		result[5] = '5';
	}
 	else if(strcmp (month,"Jun") == 0){
	 	result[4] = '0';
 		result[5] = '6';
	}
 	else if(strcmp (month,"Jul") == 0){
	 	result[4] = '0';
 		result[5] = '7';
	}
 	else if(strcmp (month,"Aug") == 0){
	 	result[4] = '0';
 		result[5] = '8';
	}
 	else if(strcmp (month,"Sep") == 0){
	 	result[4] = '0';
 		result[5] = '9';
	}
 	else if(strcmp (month,"Oct") == 0){
	 	result[4] = '1';
 		result[5] = '0';
	}
 	else if(strcmp (month,"Nov") == 0){
	 	result[4] = '1';
 		result[5] = '1';
	}
 	else if(strcmp (month,"Dec") == 0){
	 	result[4] = '1';
 		result[5] = '2';
	}	
	
	return _atoi(result);

}
 


	
