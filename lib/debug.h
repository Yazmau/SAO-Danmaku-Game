#ifndef _HEADER_

#define _HEADER_
#include "data.h"

#endif 
	

void game_vlog(char* format, va_list arg) {
	static bool is_clear = false;
	
	vprintf(format, arg);
	printf("\n");
	
	FILE* file = fopen("log.txt", !is_clear ? "w" : "a");
	if(file) {
		vfprintf(file, format, arg);
		fprintf(file, "\n");
		fclose(file);
	}
	else
		printf("fail to open the log.txt \n");
	
	is_clear = true;
}

void game_log(char* format, ...) {
	va_list arg;
	va_start(arg, format);
	game_vlog(format, arg);
	va_end(arg);
}

void exe(bool is_succesful_to_execution, char *error_message, ...) {
	if(!is_succesful_to_execution) {
		va_list arg;
		va_start(arg ,error_message);
		game_vlog(error_message, arg);
		va_end(arg);
	
		printf("something wrong\n");
		printf("exit...(2)\n");
		al_rest(1.0);
		printf("exit...(1)\n");
		al_rest(1.0);
		printf("exit...(0)\n");
		exit(1);
	}
}

