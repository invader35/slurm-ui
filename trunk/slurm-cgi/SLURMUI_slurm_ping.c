#include "slurm.h"
#include "json/json.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

int main()
{


	struct json_object *json = json_object_new_object();
	int err = SLURM_SUCCESS;
	
	err = slurm_ping(1);
  	json_object_object_add(json, "slurm_ping(1)", json_object_new_int(err));
	
	err = slurm_ping(2);
	json_object_object_add(json, "slurm_ping(2)", json_object_new_int(err));
	
	printf("Content-type: text/html\n\n%s",json_object_to_json_string(json));
	
	return 1;

}



