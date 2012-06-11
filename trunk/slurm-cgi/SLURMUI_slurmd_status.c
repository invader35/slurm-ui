#include "slurm.h"
#include "json/json.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>


int main()
{

	int err = SLURM_SUCCESS;
	int i = 0;
    
	struct json_object *json = json_object_new_object();
	slurmd_status_t *status_ptr = NULL;
	

	err = slurm_load_slurmd_status(&status_ptr);
	if (err) {
		json_object_object_add(json, "error", json_object_new_int(err));
	}

		
	json_object_object_add(json,"Booted",json_object_new_int(status_ptr->booted));
	json_object_object_add(json,"Last_Msg",json_object_new_int(status_ptr->last_slurmctld_msg));
	json_object_object_add(json,"Logging_Level", json_object_new_int(status_ptr->slurmd_debug));
	json_object_object_add(json,"Actual_CPU's", json_object_new_int(status_ptr->actual_cpus));
	json_object_object_add(json,"Actual_Sockets",json_object_new_int(status_ptr->actual_sockets));
	json_object_object_add(json,"Actual_Cores",json_object_new_int(status_ptr->actual_cores));
	json_object_object_add(json,"Actual_Threads",json_object_new_int(status_ptr->actual_threads));
	json_object_object_add(json,"Actual_Real_Mem",json_object_new_int(status_ptr->actual_real_mem));
	json_object_object_add(json,"Actual_Tmp_Disk",json_object_new_int(status_ptr->actual_tmp_disk));
	json_object_object_add(json,"PID",json_object_new_int(status_ptr->pid));
	json_object_object_add(json,"Hostname",json_object_new_string(status_ptr->hostname));
	json_object_object_add(json,"Slurm Logfile",json_object_new_string(status_ptr->slurmd_logfile));
	json_object_object_add(json,"Step List",json_object_new_string(status_ptr->step_list));
	json_object_object_add(json,"Version",json_object_new_string(status_ptr->version));
	

	if (status_ptr != NULL) {
		slurm_free_slurmd_status(status_ptr);
	}

	printf("Content-type: text/html\n\n%s",json_object_to_json_string(json));
	return 1;
}



