#include <stdio.h>
#include <stdarg.h>

#include "logger.h"

#define LOG_FILE_PATH "battletested.log"

int has_init = 0;

static int init_log(){
  	has_init = 1;

  	FILE *fp = fopen(LOG_FILE_PATH, "w");

  	if(!fp){
		fprintf(stderr, "failed to create log file");
		return 1;
	}
	
	fprintf(fp, "===============\n");
	fprintf(fp, "      LOG      \n");
	fprintf(fp, "===============\n\n");

	fclose(fp);
	return 0;
}

int logger_log(const char *format, ...){
	if (!has_init){
		init_log();
	}

	FILE *fp = fopen(LOG_FILE_PATH, "a");
	
	if (!fp)
	{
		fprintf(stderr, "failed to open log file\n");
		return 1;
	}

	char buffer[255];

	va_list argv;
	va_start(argv, format);
	vsprintf(buffer, format, argv);
	va_end(argv);

	fprintf(fp, "%s\n", buffer);

	fclose(fp);
	return 0;
}

