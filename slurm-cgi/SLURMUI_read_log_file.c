#include "SLURMUI.h"

int HASH_FULL = 0;

void parse_line(char* input, struct json_object* partition){

	char* tmp1 = NULL;
	char* current_partition = NULL;
	char* old_partition = NULL;
	char* current_node = NULL;
	char* old_node = NULL;
	char* current_job = NULL;
	char* old_job = NULL;
		
	char job_str[10000];

	struct json_object *node = json_object_new_object();

	strtok(input,"]"); 	//trim date
	strtok(NULL,"~");	//number of partition	
	
	bool first_node = true;
	bool first_partition = true;
	while(1){

		tmp1 = strtok(NULL,"~");
	
		if(*tmp1 == '\n'){

			
			if (job_str[0]=='\0') json_object_object_add(node, current_node, json_object_new_string("_"));
			else	json_object_object_add(node, current_node, json_object_new_string(job_str));
			json_object_object_add(partition, current_partition, node);
			break;
		
		}

		if(strcmp(tmp1,"PARTITION")==0){			

			tmp1 = strtok(NULL,"~");	//get partition name
			
			old_partition = current_partition;
			current_partition = tmp1;
			
			if (!first_partition ) {
			
				if(current_node != NULL){
					
					if (job_str[0]=='\0') json_object_object_add(node, current_node, json_object_new_string("_"));
					else json_object_object_add(node, current_node, json_object_new_string(job_str));
					job_str[0] = '\0';		
				}	
				json_object_object_add(partition, old_partition, node);
				node = json_object_new_object();
				first_node = true;
			}			
			else first_partition = false;		
		
		}
		else if(strcmp(tmp1,"NODE") == 0 ){

			tmp1 = strtok(NULL,"~");	//get node name		
			
			old_node = current_node;
			current_node = tmp1;			

			if (!first_node ) {
				
				if (job_str[0]=='\0') json_object_object_add(node, old_node, json_object_new_string("_"));
				else json_object_object_add(node, old_node, json_object_new_string(job_str));
				job_str[0] = '\0';		

			}
			else first_node = false;			

			
		}
		else if(strcmp(tmp1,"JOB")==0){

			tmp1 = strtok(NULL,"~");	//get job id
			
			old_job = current_job;
			current_job = tmp1;

			sprintf(job_str,"%sJob ID:%s,",job_str,tmp1);
			tmp1 = strtok(NULL,"~");	//get job name	

		}
			
	}

}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// MAIN
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

int main(){

	FILE *f;

	int i, j;
	int start_date;
	int end_date;
  	char buffer[READ_LOG_BUFFER_SIZE];


	struct json_object *line = json_object_new_object();
	struct json_object *overall = json_object_new_object();

  	f = fopen("./log/SLURMUI_log.txt", "rt");
	
	if(f == NULL){
		
		printf("Cannot open the log file!\n"); 
		return 0;
	}
	
	strtok( getenv("QUERY_STRING"),"=");
	char* teemp1 = strtok(NULL,"=");
	char* teemp2 = strtok(NULL,"=");
	char* teemp3 = strtok(NULL,"=");
	start_date= getCurrentTime2(teemp1);
	end_date = getCurrentTime2(teemp2);
	int max_number = _atoi(teemp3);

//============================================================================================
// Read log file & create output
//============================================================================================

	j=0;
	i=0;
	while(fgets(buffer, READ_LOG_BUFFER_SIZE, f)){		
		if((getCurrentTime(buffer)>=start_date) && (getCurrentTime(buffer) < end_date) && j<max_number){				
			line = json_object_new_object();			
			parse_line(buffer,line);

			char key[5];	
			sprintf(key,"%d",i++);
			json_object_object_add(overall, key, line);
			j++;	
				
		}
		else if (getCurrentTime(buffer) >= end_date)
			break;
	
		
	}
	
	printf("Content-type: text/html\n\n%s",json_object_to_json_string(overall));
	int err = fclose(f);
	if(err!=0) printf("File not close successfully\n");
	
    	
	return 1;


}
