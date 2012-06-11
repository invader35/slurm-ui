#include "../SLURMUI.h"

extern Partition_Node* node_job_hash[HASH_SIZE];
int HASH_FULL;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%n
// MAIN
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


int main(){

	FILE* fp;
	int i,j;
	int i2,j2;
	
	job_info_msg_t *job_ptr = NULL;
	node_info_msg_t *node_ptr = NULL;		
	partition_info_msg_t *prt_ptr = NULL;

	int err = SLURM_SUCCESS;
	err = slurm_load_jobs((time_t) NULL, &job_ptr, 0);		
	err = slurm_load_partitions((time_t) NULL, &prt_ptr, 0);
	err = slurm_load_node((time_t) NULL, &node_ptr, 0);

	Linked_List_Node* job_llist;
	
	fp=fopen("SLURMUI_log.txt","a");
	if( fp == NULL ) { printf("Cannot open the log file\n"); return 0;}
	
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

			int j2=0;
			
			while( prt_ptr->partition_array[i].node_inx[j2] >= 0){
				
				int i2 = 0;

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
	
	err = fclose(fp);
	if(err!=0) printf("File not close successfully\n");
	
	printf("Content-type: text/html\n\n%s",json_object_new_object());
	
	return 1;

}
