/*#include "slurm.h"
#include "json/json.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <time.h>
*/
#include "SLURMUI.h"

int HASH_FULL = 0;


int main(){


	partition_info_msg_t *prt_ptr = NULL;
	node_info_msg_t *node_ptr = NULL;

	struct json_object *overall = json_object_new_object();

	int err = SLURM_SUCCESS;
	err = slurm_load_partitions((time_t) NULL, &prt_ptr, 0);
	err = slurm_load_node((time_t) NULL, &node_ptr, 0);


//============================================================================================
// Put partition & node info to hash
//============================================================================================


	char partition_names[30];
	partition_names[0]='\0';
	char partition_number[20];
	int i = 0;	
	char node_names[50000];	
	node_names[0] = '\0';
	char node_number[200];	

	sprintf(partition_number,"partitioncount~%d",prt_ptr->record_count);
	for (i = 0; i < prt_ptr->record_count; i++) {
		
		sprintf(partition_names,"%s%s~",partition_names,prt_ptr->partition_array[i].name);
			
	}	
	json_object_object_add(overall, partition_number , json_object_new_string(partition_names));

	int j2 = 0;
	int node_counter = 0;
	if(prt_ptr -> record_count > 0){	
	
		for (i = 0; i < prt_ptr->record_count; i++) {
			
			j2=0;
			
				while( prt_ptr->partition_array[i].node_inx[j2] >= 0){
				
					int i2 = 0;
				
					for(i2 = prt_ptr->partition_array[i].node_inx[j2];i2 <= prt_ptr->partition_array[i].node_inx[j2+1];i2++) {
							
						sprintf(node_names,"%s%s~",node_names,node_ptr->node_array[i2].name);
						node_counter++;

					}
					j2 += 2;				
			
					sprintf(node_number,"partitionname~%s~%d",prt_ptr->partition_array[i].name,node_counter);
					
					json_object_object_add(overall, node_number , json_object_new_string(node_names));
					node_names[0] ='\0'; 
					node_counter = 0;				
			
				}
				
		}
	}			

	printf("Content-type: text/html\n\n%s",json_object_to_json_string(overall));
	
    	return 1;

}
	
