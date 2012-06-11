#include "slurm.h"
#include "json/json.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <time.h>


#define HASH_SIZE 200
struct Hash_Node* node_job_hash[HASH_SIZE];
int HASH_FULL = 0;

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// STRUCT DEFINITIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

struct Linked_List_Node {
 	
	node_info_t value;
	job_info_t value_job;
 	struct Linked_List_Node* next;

};

struct Hash_Node{

	char* key;
	struct Linked_List_Node* value;

};



//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// FUNCTIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
/*

void append_job(struct Linked_List_Node** llist, job_info_t* job) {
	
	struct Linked_List_Node* tmp = *llist; 
	
	//the linked list is empty
	if((*llist)==NULL){
		
		(*llist) = (struct Linked_List_Node*)malloc(sizeof(struct Linked_List_Node));
  		(*llist)->value_job = (*job);
  		(*llist)->next = NULL;
		
	}	
	else{
		
		while( tmp->next != NULL)
	  		tmp = tmp->next;

		tmp->next = (struct Linked_List_Node*)malloc(sizeof(struct Linked_List_Node));
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
		
		node_job_hash[value%HASH_SIZE] = malloc(sizeof(struct Hash_Node));
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
			
				node_job_hash[value%HASH_SIZE] = malloc(sizeof(struct Hash_Node));
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

struct Linked_List_Node* node_job_get(char* key){

	int i = 0;
	long value = 0;
 
	for( ; key[i] != '\0' ; i++){

		value += key[i];

	}
	
	//requested key not found
	if(node_job_hash[value%HASH_SIZE] == NULL) return NULL;
	
	//requested key found
	if( strcmp(key,node_job_hash[value%HASH_SIZE]-> key) == 0)
		return (node_job_hash[value%HASH_SIZE]->value);

	//keep searching the key	
	else{
	
		i=1;

		while(i<HASH_SIZE){

			value += i;	
			if(node_job_hash[value%HASH_SIZE] == NULL) return NULL;
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
*/

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// MAIN
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


int main(){

	FILE* fp;
	int i,j;
	
	job_info_msg_t *job_ptr = NULL;
	node_info_msg_t *node_ptr = NULL;		
	partition_info_msg_t *prt_ptr = NULL;

	int err = SLURM_SUCCESS;
	err = slurm_load_jobs((time_t) NULL, &job_ptr, 0);		
	err = slurm_load_partitions((time_t) NULL, &prt_ptr, 0);
	err = slurm_load_node((time_t) NULL, &node_ptr, 0);

	struct Linked_List_Node* job_llist;
	struct Linked_List_Node* node_llist;
	
	printf("Content-type: text/html\n\n");
	fp=fopen("SLURMUI_log.txt","a");
	if( fp == NULL ) { printf("Cannot open the log file\n"); return 0;}
	
//===========================================================================================
// Filling hash tables
//===========================================================================================

int j2 = 0;
int i2 = 0;

	//fill node_job hash
	if (job_ptr->record_count > 0) {

		for (i = 0; i < job_ptr->record_count; i++) {
			
			
			j2=0;
			while( job_ptr->job_array[i].node_inx[j2] >= 0){
				
				i2 = 0;
								
				for(i2 = job_ptr->job_array[i].node_inx[j2];i2 <= job_ptr->job_array[i].node_inx[j2+1];i2++) {

					node_job_put(node_ptr->node_array[i2].name,job_ptr -> job_array[i]);
				}
				j2+=2;
			}
		
		}
	}
	
	//for(i=0;i<HASH_SIZE;i++){if(node_job_hash[i] != NULL) printf("\n%s , %d\n",node_job_hash[i]->key,node_job_hash[i]->value->value_job.job_id);}	


//============================================================================================
// Creating Output String
//============================================================================================
	
	char output[1000] = "";
	char* current_time;
 	time_t timer;
	timer = time(NULL);
	current_time = ctime(&timer);
	
	current_time[24]='\0';
	sprintf(output,"[%s] ~",current_time);

	if(prt_ptr -> record_count > 0){	
	
		
		for (i = 0; i < prt_ptr->record_count; i++) {
			
			sprintf(output,"%sPARTITION~%s~",output,prt_ptr->partition_array[i].name);

			j2=0;
			
			while( prt_ptr->partition_array[i].node_inx[j2] >= 0){
				
				i2 = 0;

				for(i2 = prt_ptr->partition_array[i].node_inx[j2];i2 <= prt_ptr->partition_array[i].node_inx[j2+1];i2++) {
			
				
					sprintf(output,"%sNODE~%s~",output,node_ptr->node_array[i2].name);

					job_llist = node_job_get(node_ptr->node_array[i2].name); 	//get job name					
													
					while( job_llist != NULL){
									
						sprintf(output,"%sJOB~%d~",output,job_llist->value_job.job_id);
						sprintf(output,"%s%s~",output,job_llist->value_job.name);
						
						job_llist = job_llist->next;			
							
					}
				}
				j2+=2;		
	
			}

		}	

	}

	fprintf(fp,"%s\n",output);
	
	printf("%s--",output);
	err = fclose(fp);
	if(err!=0) printf("File not close successfully\n");

	
	return 1;

}
