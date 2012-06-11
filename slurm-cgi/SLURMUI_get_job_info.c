#include "SLURMUI.h"

extern Partition_Node* node_job_hash[];
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
	struct json_object *array = json_object_new_object();


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
	
	char partition_names[30];
	partition_names[0]=' ';
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

					char state[20];			

					job_llist = node_job_get(node_ptr->node_array[i2].name); 	//get job name					
													
					while( job_llist != NULL){
	
						array = json_object_new_object();
						json_object_object_add(array, "JobName", json_object_new_string(job_llist->value_job.name));
						json_object_object_add(array, "UserId", json_object_new_int(job_llist->value_job.user_id));
						json_object_object_add(array, "Partition", json_object_new_string(job_llist->value_job.partition));
						json_object_object_add(array, "LeadingNodeName", json_object_new_string(node_ptr->node_array[i2].name));
						if(job_llist->value_job.job_state == 0 || job_llist->value_job.job_state == 8 || job_llist->value_job.job_state == 2)
						json_object_object_add(array, "JobState", json_object_new_string("waiting"));
						else if(job_llist->value_job.job_state == 1) 
						json_object_object_add(array, "JobState", json_object_new_string("running"));
						else 
						json_object_object_add(array, "JobState", json_object_new_string("stopped"));
						json_object_object_add(array, "AllocatorNode", json_object_new_string(job_llist->value_job.alloc_node));
//						json_object_object_add(array, "Account", json_object_new_string(job_llist->value.account));
//						json_object_object_add(array, "Alloc_sid", json_object_new_int(job_ptr->job_array[i].alloc_sid));
//					        json_object_object_add(array, "Assoc_id", json_object_new_int(job_ptr->job_array[i].assoc_id));
						if(job_ptr->job_array[i].batch_flag == 1) json_object_object_add(array, "BatchFlag", 	json_object_new_string("true"));
						else json_object_object_add(array, "BatchFlag", json_object_new_string("false"));
						
//						json_object_object_add(array, "Batch_host", json_object_new_string(job_ptr->job_array[i].batch_host));
//						json_object_object_add(array, "Batch_script", json_object_new_string(job_ptr->job_array[i].batch_script));
//						json_object_object_add(array, "Command", json_object_new_string(job_ptr->job_array[i].command));
//						json_object_object_add(array, "Comment", json_object_new_string(job_ptr->job_array[i].comment));
//						json_object_object_add(array, "Contiguous", json_object_new_int(job_ptr->job_array[i].contiguous));
//						json_object_object_add(array, "CPUs Per Task", json_object_new_int(job_ptr->job_array[i].cpus_per_task));
//						json_object_object_add(array, "Dependency", json_object_new_string(job_ptr->job_array[i].dependency));
//						json_object_object_add(array, "Derived ec", json_object_new_int(job_ptr->job_array[i].derived_ec));
//						json_object_object_add(array, "Eligible time", json_object_new_string((char*)job_ptr->job_array[i].eligible_time));
//						json_object_object_add(array, "End time", json_object_new_string((char*)job_ptr->job_array[i].end_time));
//						json_object_object_add(array, "Exc_nodes", json_object_new_string(job_ptr->job_array[i].exc_nodes));
//						json_object_object_add(array, "Exc_node_inx", json_object_new_int(job_ptr->job_array[i].exc_node_inx));
//						json_object_object_add(array, "Exit code", json_object_new_int(job_ptr->job_array[i].exit_code));
//						json_object_object_add(array, "Features", json_object_new_string(job_ptr->job_array[i].features));
//						json_object_object_add(array, "Gres", json_object_new_string(job_ptr->job_array[i].gres));
//						json_object_object_add(array, "Group_id", json_object_new_int(job_ptr->job_array[i].group_id));
//						json_object_object_add(array, "licenses", json_object_new_string(job_ptr->job_array[i].licenses));
						json_object_object_add(array, "Max_#ofCpu's", json_object_new_int(job_llist->value_job.max_cpus));
//						json_object_object_add(array, "max_nodes", json_object_new_int(job_ptr->job_array[i].max_nodes));
//						json_object_object_add(array, "sockets_per_node", json_object_new_int(job_ptr->job_array[i].sockets_per_node));
//						json_object_object_add(array, "cores_per_socket", json_object_new_int(job_ptr->job_array[i].cores_per_socket));
//						json_object_object_add(array, "threads_per_core", json_object_new_int(job_ptr->job_array[i].threads_per_core));
						
//						json_object_object_add(array, "network", json_object_new_string(job_ptr->job_array[i].network));
					//	json_object_object_add(array, "Nodes", json_object_new_string(job_ptr->job_array[i].nodes));
//						json_object_object_add(array, "nice", json_object_new_int(job_ptr->job_array[i].nice));
//						json_object_object_add(array, "node_inx", json_object_new_int(job_ptr->job_array[i].node_inx));
//						json_object_object_add(array, "ntasks_per_core", json_object_new_int(job_ptr->job_array[i].ntasks_per_core));
//						json_object_object_add(array, "ntasks_per_node", json_object_new_int(job_ptr->job_array[i].ntasks_per_node));
//						json_object_object_add(array, "ntasks_per_socket", json_object_new_int(job_ptr->job_array[i].ntasks_per_socket));
//						json_object_object_add(array, "num_nodes", json_object_new_int(job_ptr->job_array[i].num_nodes));
//						json_object_object_add(array, "num_cpus", json_object_new_int(job_ptr->job_array[i].num_cpus));
						
						if(job_llist->value_job.pn_min_memory == 0) json_object_object_add(array, "MinRealMem_perNode", json_object_new_string("default"));
						else json_object_object_add(array,"MinRealMem_perNode", json_object_new_int(job_llist->value_job.pn_min_memory));
                                                
                                                
						if(job_ptr->job_array[i].pn_min_cpus == 0) json_object_object_add(array, "Min_#ofCpu's_perNode", json_object_new_string("default"));
						else json_object_object_add(array, "Min_#ofCpu's_perNode", json_object_new_int(job_llist->value_job.pn_min_cpus));
						
						
						if(job_ptr->job_array[i].pn_min_memory == 0) json_object_object_add(array, "MinTmpDisk_perNode", json_object_new_string("default"));
						else json_object_object_add(array, "MinTmpDisk_perNode", json_object_new_int(job_llist->value_job.pn_min_tmp_disk)); 
//						json_object_object_add(array, "pre_sus_time", json_object_new_string((char*)job_ptr->job_array[i].pre_sus_time));
//						json_object_object_add(array, "priority", json_object_new_int(job_ptr->job_array[i].priority));
//						json_object_object_add(array, "qos", json_object_new_string(job_ptr->job_array[i].qos));
//						json_object_object_add(array, "req_nodes", json_object_new_string(job_ptr->job_array[i].req_nodes));
//						json_object_object_add(array, "req_node_inx", json_object_new_int(job_ptr->job_array[i].req_node_inx));			
//						json_object_object_add(array, "req_switch", json_object_new_int(job_ptr->job_array[i].req_switch));
//						json_object_object_add(array, "requeue", json_object_new_int(job_ptr->job_array[i].requeue));
//						json_object_object_add(array, "resize_time", json_object_new_string((char*)job_ptr->job_array[i].resize_time));
//						json_object_object_add(array, "restart_cnt", json_object_new_int(job_ptr->job_array[i].restart_cnt));
//						json_object_object_add(array, "resv_name", json_object_new_string(job_ptr->job_array[i].resv_name));
//						json_object_object_add(array, "shared", json_object_new_int(job_ptr->job_array[i].shared));
//						json_object_object_add(array, "show_flags", json_object_new_int((char*)job_ptr->job_array[i].show_flags));
//						json_object_object_add(array, "start_time", json_object_new_string((char*)job_ptr->job_array[i].start_time));
//						json_object_object_add(array, "state_desc", json_object_new_string(job_ptr->job_array[i].state_desc));
//						json_object_object_add(array, "state_reason", json_object_new_int(job_ptr->job_array[i].state_reason));
//						json_object_object_add(array, "submit_time", json_object_new_string((char*)job_ptr->job_array[i].submit_time));			
//						json_object_object_add(array, "suspend_time", json_object_new_int(job_ptr->job_array[i].suspend_time));
//						json_object_object_add(array, "time_limit", json_object_new_string(job_ptr->job_array[i].time_limit));
//						json_object_object_add(array, "time_min", json_object_new_int(job_ptr->job_array[i].time_min));
//						json_object_object_add(array, "preempt_time", json_object_new_string((char*)job_ptr->job_array[i].preempt_time));
//						json_object_object_add(array, "wait4switch", json_object_new_string(job_ptr->job_array[i].wait4switch));
//						json_object_object_add(array, "wckey", json_object_new_int(job_ptr->job_array[i].wckey));
//						json_object_object_add(array, "work_dir", json_object_new_string((char*)job_ptr->job_array[i].work_dir));									

//						json_object_object_add(array, "JobId", json_object_new_int(job_llist->value_job.job_id));
						char state[100];		
						if(job_llist->value_job.job_state == 0 || job_llist->value_job.job_state == 8 || job_llist->value_job.job_state == 2) sprintf(state,"waiting~%d",job_llist->value_job.job_id);
						else if(job_llist->value_job.job_state == 1) sprintf(state,"running~%d",job_llist->value_job.job_id);
						else sprintf(state,"stopped~%d",job_llist->value_job.job_id);
						

						json_object_object_add(node, state, array);
						job_llist = job_llist->next;
					
					}

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
