#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#include "logger.h"
#include "util.h"

#define TIME_BUFFER 26

int has_init = 0;
char log_file[BUFFER_SIZE];

static int init_log(){
  	has_init = 1;

	char time_buffer[TIME_BUFFER];
	time_t timer;
    struct tm* tm_info;

    time(&timer);
    tm_info = localtime(&timer);

    strftime(time_buffer, TIME_BUFFER, "%Y-%m-%d_%H.%M.%S.log", tm_info);

	sprintf(log_file, "log/%s", time_buffer);
  	FILE *fp = fopen(log_file, "w");

  	if(!fp){
		fprintf(stderr, "failed to create log file");
		return 1;
	}

	fclose(fp);
	return 0;
}

void no_log(){
	has_init = -1;
}

int logger(const char *format, ...){

	if(has_init == 0){
		init_log();
		logger("Initializing log.");
	}

  if(has_init > -1){

		FILE *fp = fopen(log_file, "a");

		if (!fp)
		{
			fprintf(stderr, "failed to open log file\n");
			return 1;
		}
		char time_buffer[TIME_BUFFER];
		char buffer[BUFFER_SIZE];

		va_list argv;
		va_start(argv, format);
		vsprintf(buffer, format, argv);
		va_end(argv);

		time_t timer;
		struct tm* tm_info;

		time(&timer);
		tm_info = localtime(&timer);

		strftime(time_buffer, TIME_BUFFER, "%Y-%m-%d %H:%M:%S", tm_info);

		fprintf(fp, "[%s] %s\n", time_buffer, buffer);

		fclose(fp);
	}

	return 0;
}
