#include <stdio.h>
#include <stdarg.h>

#include "logger.h"
#include "util.h"

int has_init = 0;
char *path;

static int init_log(){
  	has_init = 1;
	
	path = "log/"LOG_FILE_PATH;
	
  	FILE *fp = fopen(path, "w");

  	if(!fp){
		path = LOG_FILE_PATH;
		return 1;
	}

	fclose(fp);
	return 0;
}
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#include "logger.h"
#include "util.h"


int logger(const char *format, ...){

	if(!has_init){
		init_log();
		logger("Initializing log.");
	}
	
	FILE *fp = fopen(LOG_FILE_PATH, "a");
	
	if (!fp)
	{
		fprintf(stderr, "failed to open log file\n");
		return 1;
	}
	char time_buffer[26];
	char buffer[255];

	va_list argv;
	va_start(argv, format);
	vsprintf(buffer, format, argv);
	va_end(argv);
	
	time_t timer;
    struct tm* tm_info;

    time(&timer);
    tm_info = localtime(&timer);

    strftime(time_buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
	
	fprintf(fp, "[%s] %s\n", time_buffer, buffer);

	fclose(fp);
	return 0;
}


