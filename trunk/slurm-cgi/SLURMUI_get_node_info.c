#include "SLURMUI.h"

extern Partition_Node* node_job_hash[];
int HASH_FULL = 0;


int main()
{

//===========================================================================================
// Declarations
//===========================================================================================
	
	int i,j;
	
	job_info_msg_t *job_ptr;	
	partition_info_msg_t *prt_ptr = NULL;
	node_info_msg_t *node_ptr = NULL;

	int err = SLURM_SUCCESS;
	err = slurm_load_partitions((time_t) NULL, &prt_ptr, 0);
	err = slurm_load_node((time_t) NULL, &node_ptr, 0);
	err = slurm_load_jobs((time_t) NULL, &job_ptr, 0);	

	Linked_List_Node* job_llist;
	//struct Linked_List_Node* node_llist;
	struct json_object *partition = json_object_new_object();
	struct json_object *node = json_object_new_object();
	struct json_object *array ;


//===========================================================================================
// Filling hash tables
//===========================================================================================
	int j2;
	int i2;

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
// Creating Output in Json
//============================================================================================

	// create json	
	
	char partition_names[30];
	partition_names[0] = ' ';
	for (i = 0; i < prt_ptr->record_count; i++) {
		
		sprintf(partition_names,"%s%s~",partition_names,prt_ptr->partition_array[i].name);
			
	}	

	json_object_object_add(partition,  partition_names, json_object_new_int(prt_ptr->record_count));	
	

	if(prt_ptr -> record_count > 0){	
	
		for (i = 0; i < prt_ptr->record_count; i++) {
			
			
			j2=0;
			while( prt_ptr->partition_array[i].node_inx[j2] >= 0){
				
				i2 = 0;
				node = json_object_new_object();
				for(i2 = prt_ptr->partition_array[i].node_inx[j2];i2 <= prt_ptr->partition_array[i].node_inx[j2+1];i2++) {

					array = json_object_new_object();
					char state[20];
					char jobs[100];
					jobs[0] = '\0';			

					job_llist = node_job_get(node_ptr->node_array[i2].name); 	//get job name					
					while( job_llist != NULL){
									
						sprintf(jobs,"%s %d,",jobs,job_llist->value_job.job_id );
						job_llist = job_llist->next;			
								
					}		
					
					json_object_object_add(array, "NodeName",json_object_new_string(node_ptr->node_array[i2].name));	
					json_object_object_add(array, "CommunicationName", json_object_new_string(node_ptr->node_array[i2].node_addr));				
					json_object_object_add(array, "Jobs", json_object_new_string(jobs));	
//					json_object_object_add(array, "Arch", json_object_new_string(node_ptr->node_array[i2].arch));
					json_object_object_add(array, "RealMemory", json_object_new_int(node_ptr->node_array[i2].real_memory));
					json_object_object_add(array, "TmpDisk", json_object_new_int(node_ptr->node_array[i2].tmp_disk));
//					json_object_object_add(array, "Boot_time", json_object_new_string((char*)node_ptr->node_array[i2].boot_time));
					json_object_object_add(array, "ArbitraryPriority", json_object_new_int(node_ptr->node_array[i2].weight));	
					json_object_object_add(array, "Cpu's", json_object_new_int(node_ptr->node_array[i2].cpus));
					json_object_object_add(array, "#ofCores_perCpu", json_object_new_int(node_ptr->node_array[i2].cores));
//					json_object_object_add(array, "Features", json_object_new_string(node_ptr->node_array[i2].features));
//					json_object_object_add(array, "GenericResources", json_object_new_string(node_ptr->node_array[i2].gres));
//					json_object_object_add(array, "OS", json_object_new_string(node_ptr->node_array[i2].os));
//					json_object_object_add(array, "Reason", json_object_new_string(node_ptr->node_array[i2].reason));
//					json_object_object_add(array, "ReasonTime", json_object_new_string((char*)node_ptr->node_array[i2].reason_time));
//					json_object_object_add(array, "ReasonUid", json_object_new_int(node_ptr->node_array[i2].reason_uid));
//					json_object_object_add(array, "SlurmdStarTime", json_object_new_string((char*)node_ptr->node_array[i2].slurmd_start_time));
					json_object_object_add(array, "#ofSockets_perNode", json_object_new_int(node_ptr->node_array[i2].sockets));
					json_object_object_add(array, "#ofThreads_perCore", json_object_new_int(node_ptr->node_array[i2].threads));
					
					if(node_ptr->node_array[i2].node_state == 5 || node_ptr->node_array[i2].node_state == 3) sprintf(state,"allocated~%s",node_ptr->node_array[i2].name);
					else if(node_ptr->node_array[i2].node_state == 1) sprintf(state,"down~%s",node_ptr->node_array[i2].name);
					else if(node_ptr->node_array[i2].node_state == 2 || node_ptr->node_array[i2].node_state ==6) sprintf(state,"idle~%s",node_ptr->node_array[i2].name);
					else sprintf(state,"unknown~%s",node_ptr->node_array[i2].name);

					json_object_object_add(node, state, array);
				}	
				j2+=2;		
	
			}

			json_object_object_add(partition, prt_ptr->partition_array[i].name , node);
			

		}	

	}

	printf("Content-type: text/html\n\n%s",json_object_to_json_string(partition));

	slurm_free_partition_info_msg(prt_ptr);
	slurm_free_node_info_msg(node_ptr);
	slurm_free_job_info_msg(job_ptr);
	return 1;


}
