#include "SLURMUI.h"


int main(){
	
	int err = SLURM_SUCCESS;
	int i;
        char *tmp = NULL;
	partition_info_msg_t *prt_ptr = NULL;
	struct json_object *json = json_object_new_object();
	struct json_object *array;

	err = slurm_load_partitions((time_t) NULL, &prt_ptr, 0);

	if (err) {
		json_object_object_add(json, "error", json_object_new_int(err));
		return 0;
	}
	
	// create json	
	
	char partition_names[100];
	partition_names[0]=' ';
	for (i = 0; i < prt_ptr->record_count; i++) {
		
		sprintf(partition_names,"%s%s~",partition_names,prt_ptr->partition_array[i].name);
			
	}	

	json_object_object_add(json,  partition_names, json_object_new_int(prt_ptr->record_count));	

	for (i = 0; i < prt_ptr->record_count; i++) {

		array = json_object_new_object();

		json_object_object_add(array, "Allowed Allocating Groups", json_object_new_string(prt_ptr->partition_array[i].allow_alloc_nodes));
//  		json_object_object_add(array, "List of Groups", json_object_new_string(prt_ptr->partition_array[i].allow_groups));
//		json_object_object_add(array, "Alternate", json_object_new_string(prt_ptr->partition_array[i].alternate));
		json_object_object_add(array, "Default Allocated Memory Per CPU", json_object_new_int(prt_ptr->partition_array[i].def_mem_per_cpu));
		json_object_object_add(array, "Default Time", json_object_new_int(prt_ptr->partition_array[i].default_time));
		json_object_object_add(array, "Flags", json_object_new_int(prt_ptr->partition_array[i].flags));
		json_object_object_add(array, "Grace Time", json_object_new_int(prt_ptr->partition_array[i].grace_time));
		json_object_object_add(array, "Max Memory Per CPU", json_object_new_int(prt_ptr->partition_array[i].max_mem_per_cpu));
		json_object_object_add(array, "Max Nodes", json_object_new_int(prt_ptr->partition_array[i].max_nodes));
		json_object_object_add(array, "Max Share", json_object_new_int(prt_ptr->partition_array[i].max_share));
		json_object_object_add(array, "Max Time", json_object_new_int(prt_ptr->partition_array[i].max_time));
		json_object_object_add(array, "Min Nodes", json_object_new_int(prt_ptr->partition_array[i].min_nodes));
//		json_object_object_add(array, "Node Index", json_object_new_int((prt_ptr->partition_array[i].node_inx)));
//		json_object_object_add(array, "Node Names", json_object_new_string(prt_ptr->partition_array[i].nodes));
		json_object_object_add(array, "Preempt Mode", json_object_new_int(prt_ptr->partition_array[i].preempt_mode));
		json_object_object_add(array, "Priority", json_object_new_int(prt_ptr->partition_array[i].priority));
		json_object_object_add(array, "State Up", json_object_new_int(prt_ptr->partition_array[i].state_up));	
		json_object_object_add(array, "Total CPUs", json_object_new_int(prt_ptr->partition_array[i].total_cpus));
		json_object_object_add(array, "Total Nodes", json_object_new_int(prt_ptr->partition_array[i].total_nodes));
		
		json_object_object_add(json, prt_ptr->partition_array[i].name, array);

	}


	printf("Content-type: text/html\n\n%s",json_object_to_json_string(json));
	slurm_free_partition_info_msg(prt_ptr);
	return 1;

}
