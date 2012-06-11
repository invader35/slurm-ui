#include "slurm.h"
#include "json/json.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <time.h>

#ifndef HASH_SIZE
#define HASH_SIZE 200
#endif

#ifndef READ_LOG_BUFFER_SIZE 
#define READ_LOG_BUFFER_SIZE 10000
#endif



//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// STRUCT DEFINITIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

typedef struct _Linked_List_Node {
 	
	node_info_t value;
	job_info_t value_job;
        struct	_Linked_List_Node* next;

}Linked_List_Node;

typedef struct _Partition_Node{

	char* key;
	Linked_List_Node* value;

}Partition_Node;

typedef struct _Node_Name_Info{

	char** names;
	int number_of_nodes;

}Node_Name_Info;



//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// FUNCTIONS DEFINITIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

int _atoi(const char* string);

Linked_List_Node* node_job_get(char* key);

void append_job(Linked_List_Node** llist, job_info_t* job);

void node_job_put(char* key, job_info_t job);

int getCurrentTime(char* input);

int getCurrentTime2(char* input);
