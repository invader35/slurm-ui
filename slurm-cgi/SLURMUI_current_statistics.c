#include "SLURMUI.h"

Partition_Node* job_node_hash[HASH_SIZE];
int HASH_FULL = 0;


int main()
{

//===========================================================================================
// Declarations
//===========================================================================================
	
	int i,j;
	int i2,j2;
	
	job_info_msg_t *job_ptr;	
	partition_info_msg_t *prt_ptr = NULL;
	node_info_msg_t *node_ptr = NULL;

	int err = SLURM_SUCCESS;
	err = slurm_load_partitions((time_t) NULL, &prt_ptr, 0);
	err = slurm_load_node((time_t) NULL, &node_ptr, 0);
	err = slurm_load_jobs((time_t) NULL, &job_ptr, 0);	

	Linked_List_Node* job_llist;
	struct json_object *partition = json_object_new_object();
	struct json_object *node = json_object_new_object();


//===========================================================================================
// Filling hash tables
//===========================================================================================

	j2 = 0;	
	i2 = 0;

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
	

//============================================================================================
// Creating Output in Json
//============================================================================================

	// create json	

	int total_node_unknown = 0;
	int total_node_down = 0;
	int total_node_idle = 0;
	int total_node_allocated = 0;
	
	int current_node_unknown;
	int current_node_down;
	int current_node_idle;
	int current_node_allocated;

	int current_job_running;
	int current_job_waiting;
	int current_job_stopped;


	if(prt_ptr -> record_count > 0){	
	
		for (i = 0; i < prt_ptr->record_count; i++) {


			current_node_unknown = 0;
			current_node_down = 0;
			current_node_idle = 0;
			current_node_allocated = 0;

			current_job_running = 0;
			current_job_waiting = 0;			
			current_job_stopped = 0;
			
			
			int j2=0;
			while( prt_ptr->partition_array[i].node_inx[j2] >= 0){
				
				int i2 = 0;

				for(i2 = prt_ptr->partition_array[i].node_inx[j2];i2 <= prt_ptr->partition_array[i].node_inx[j2+1];i2++) {


					if(node_ptr->node_array[i2].node_state == 5 || node_ptr->node_array[i2].node_state == 3){   total_node_allocated++;   current_node_allocated++;}
					else if(node_ptr->node_array[i2].node_state == 1){ 	total_node_down++;      current_node_down++;}
					else if(node_ptr->node_array[i2].node_state == 2 || node_ptr->node_array[i2].node_state == 6){ 	total_node_idle++;      current_node_idle++;}
					else { 	total_node_unknown++; current_node_unknown++;}
	
					job_llist = node_job_get(node_ptr->node_array[i2].name); 			//get job name	
					while( job_llist != NULL){
						

						if(job_llist->value_job.job_state == 2 || job_llist->value_job.job_state == 8 || job_llist->value_job.job_state == 0) 	current_job_waiting++;
						else if(job_llist->value_job.job_state == 1) 	current_job_running++;
						else	current_job_stopped++;
					
						job_llist = job_llist->next;			
									
					}
				}	
				j2+=2;		
	
			}
			
			json_object_object_add(node, "Allocated", json_object_new_int(current_node_allocated));
			json_object_object_add(node, "Down", json_object_new_int(current_node_down));
			json_object_object_add(node, "Idle", json_object_new_int(current_node_idle));
			json_object_object_add(node, "Unknown", json_object_new_int(current_node_unknown));
			json_object_object_add(node, "Running", json_object_new_int(current_job_running));
			json_object_object_add(node, "Wating", json_object_new_int(current_job_waiting));
			json_object_object_add(node, "Stopped", json_object_new_int(current_job_stopped));

			json_object_object_add(partition, prt_ptr->partition_array[i].name , node);

		}
	
	}		
	
	node = json_object_new_object();
	json_object_object_add(node, "Unknown", json_object_new_int(total_node_unknown));
	json_object_object_add(node, "Down", json_object_new_int(total_node_down));
	json_object_object_add(node, "Idle", json_object_new_int(total_node_idle));
	json_object_object_add(node, "Allocated", json_object_new_int(total_node_allocated));
	
	json_object_object_add(partition, "General", node);
	printf("Content-type: text/html\n\n%s",json_object_to_json_string(partition));

	slurm_free_partition_info_msg(prt_ptr);
	slurm_free_node_info_msg(node_ptr);
	return 1;


}
