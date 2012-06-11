#include "SLURMUI.h"

int HASH_FULL;

int main(){

	FILE *f;

	size_t date_time = 26;
  	char input[READ_LOG_BUFFER_SIZE];

	struct json_object *overall = json_object_new_object();

  	f = fopen("SLURMUI_log.txt", "rt");
	
	if(f == NULL){
		
		printf("Cannot open the log file!\n"); 
		return 0;
	}
	

//============================================================================================
// Read log file & create output
//============================================================================================

	if(fgets(input, READ_LOG_BUFFER_SIZE, f)){		
	
       
		char *ptr;
 		char month[3];
		char result[10];
 
 		month[0] = input[5];
 		month[1] = input[6];
 		month[2] = input[7]; 
		month[3] = '\0';
 
		result[0] = input[21];
	 	result[1] = input[22];
		result[2] = input[23];
	 	result[3] = input[24];
		result[4] = '-';
 	
		result[7] = '-';
		if(input[9] == ' ') result[8] = '0';
		else 		    result[8] = input[9];
		result[9] = input[10];
		result[10] = '\0';

 		if(strcmp (month,"Jan") == 0){
 
		 	result[5] = '0';
 			result[6] = '1';
 		
		}
 		else if(strcmp (month,"Feb") == 0){
	 		result[5] = '0';
	 		result[6] = '2';
		}
 		else if(strcmp (month,"Mar") == 0){
	 		result[5] = '0';
 			result[6] = '3';
		}
	 	else if(strcmp (month,"Apr") == 0){
		 	result[5] = '0';
	 		result[6] = '4';
		}
 		else if(strcmp (month,"May") == 0){
	 		result[5] = '0';
 			result[6] = '5';
		}
 	else if(strcmp (month,"Jun") == 0){
	 	result[5] = '0';
 		result[6] = '6';
	}
 	else if(strcmp (month,"Jul") == 0){
	 	result[5] = '0';
 		result[6] = '7';
	}
 	else if(strcmp (month,"Aug") == 0){
	 	result[5] = '0';
 		result[6] = '8';
	}
 	else if(strcmp (month,"Sep") == 0){
	 	result[5] = '0';
 		result[6] = '9';
	}
 	else if(strcmp (month,"Oct") == 0){
	 	result[5] = '1';
 		result[6] = '0';
	}
 	else if(strcmp (month,"Nov") == 0){
	 	result[5] = '1';
 		result[6] = '1';
	}
 	else if(strcmp (month,"Dec") == 0){
	 	result[5] = '1';
 		result[6] = '2';
	}

	
		json_object_object_add(overall, "Start Time",json_object_new_string(result));			
		
	}

	
	int err = fclose(f);
	if(err!=0) printf("File not close successfully\n");
	
	printf("Content-type: text/html\n\n%s",json_object_to_json_string(overall));

	return 1;

}	
